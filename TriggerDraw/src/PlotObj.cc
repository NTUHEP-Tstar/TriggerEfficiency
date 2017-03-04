#include "TriggerEfficiency/TriggerDraw/interface/PlotObj.hpp"
#include "ManagerUtils/PlotUtils/interface/Common.hpp"
#include "TriggerEfficiency/TriggerDraw/interface/eConstant.hpp"

/*******************************************************************************
*   Global variable
*******************************************************************************/

Color_t color[10] = {kRed+2,kBlue-7,kGreen+3,kMagenta+2,kYellow-3,kGreen-9,kBlue-9,kRed-9,kCyan+3,kGray };

Style_t  mstyle[10] = {20,21,24,25,31,33,27,23,22,26};

dra::Triggermgr trinamer("TriggerDraw");


/*******************************************************************************
*   Global function
*******************************************************************************/

extern void SaveToRoot(TH1* h, const string& fname, const string& objname){
    TFile f(fname.c_str(),"UPDATE");
    h->Write( objname.c_str(), TFile::kOverwrite  );

}

extern void Clean(){
    delete trinamer.GetDataPt();
    delete trinamer.GetDataEta();
    delete trinamer.GetMCPt();
    delete trinamer.GetMCEta();

    trinamer.SetPtData ( NULL);
    trinamer.SetEtaData( NULL);
    trinamer.SetPtMC   ( NULL);
    trinamer.SetEtaMC  ( NULL);

}

extern TH1D* SetComGraph(vector<double>& _bin, TGraph* dEff, TGraph* mEff){
   
    double* bin    = &_bin[0];
    double  binnum = _bin.size();
    

    TH1D* ratio = new TH1D("ratio","",binnum-1,bin);
            
    for(int j=1;j<binnum;j++){
        double deff = dEff->GetY()[j-1];
        double meff = mEff->GetY()[j-1];
        double derr = dEff->GetErrorY(j-1);
        double merr = mEff->GetErrorY(j-1);
        double seff;
        double serr;

        if(meff!=0){
                seff= deff/meff;
                serr = dra::ErrorProp(deff,derr,meff,merr);
            }
            else{
                seff=0;
                serr=0;
            }
        
        ratio->SetBinContent(j,seff);
        ratio->SetBinError(j,serr);    
    }

    ratio->SetMarkerStyle(21);
    ratio->SetStats(kFALSE);
    return ratio;
}

extern void SetGraph(TGraph* mEff, TGraph* dEff){

    mEff->SetLineColor(color[0]);
    mEff->SetMarkerColor(color[0]);
    mEff->SetMarkerStyle(mstyle[0]);
    mEff->SetMarkerSize(0.6);

    dEff->SetLineColor(color[1]);
    dEff->SetMarkerColor(color[1]);
    dEff->SetMarkerStyle(mstyle[1]);
    dEff->SetMarkerSize(0.6);
}

extern void SetHist(TH1* h, const string& x, const string& y){
    
    plt::SetAxis(h);
    h->GetXaxis()->SetTitle(x.c_str());
    h->GetYaxis()->SetTitle(y.c_str());
    h->GetXaxis()->SetLabelSize(0 );

}

extern void SetGraphName(const string& tri){

    trinamer.GetDataPt()->SetName( ("d"+tri).c_str()  );
    trinamer.GetMCPt()  ->SetName( ("m"+tri).c_str()  );
    trinamer.GetDataEta()->SetName( ("d"+tri).c_str()  );
    trinamer.GetMCEta()  ->SetName( ("m"+tri).c_str()  );
}

extern TLine* SetTLine(const double& xmin, const double& ymin, const double& xmax, const double& ymax ){
    TLine* line = new TLine(xmin,ymin,xmax,ymax);
    line->SetLineColor(kRed);
    line->SetLineStyle(8);
    line->Draw();
    return line;
}

extern TPaveText* SetTPave(const double& xmin, const double& ymin, const double& xmax, const double& ymax){
    TPaveText *pt = plt::NewTextBox(xmin,ymin,xmax,ymax);
    pt->AddText(_text);
    pt->Draw();
    return pt;
}

extern TLegend* SetTLeg(const string& tri,const string& triname,const string& cut,const double& xmin,const double& ymin,const double& xmax,const double& ymax){
    
    TLegend* leg = plt::NewLegend(xmin,ymin,xmax,ymax);
    leg->SetLineColor(kWhite);
    leg->AddEntry( ("m"+tri).c_str(), ( "MC   "+triname+" ("+cut+")" ).c_str(), "lp");
    leg->AddEntry( ("d"+tri).c_str(), ( "Data "+triname+" ("+cut+")" ).c_str(), "lp");
    leg->SetTextSize(14);
    leg->Draw();
    return leg;
}
