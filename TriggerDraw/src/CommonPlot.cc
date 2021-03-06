#include "TriggerEfficiency/TriggerDraw/interface/PlotObj.hpp"
#include "ManagerUtils/PlotUtils/interface/Common.hpp"
#include "TriggerEfficiency/TriggerDraw/interface/eConstant.hpp"
#include <boost/algorithm/string.hpp>
#include <iostream>

/*******************************************************************************
*   Global variable
*******************************************************************************/

Color_t color[10] = {kRed+2,kBlue-7,kGreen+3,kMagenta+2,kYellow-3,kGreen-9,kBlue-9,kRed-9,kCyan+3,kGray };

Style_t  mstyle[10] = {20,21,24,25,31,33,27,23,22,26};

dra::Triggermgr trinamer("TriggerDraw");

double lumi = 0;
string _text = "Electron run ";
/*******************************************************************************
*   Global function
*******************************************************************************/
extern void CalcLumi(){
    string run = trinamer.GetOption<string>("run");
   
    
    if(run == "all"){
        run = "bcdefgh";
    }

    for(auto& r : run){
        string s="";
        s+=r;
        lumi += dra::GetSingle<double>( s  ,trinamer.GetSubTree("lumi"));
    }

    _text += SetEra(run);
}

extern string SetEra( string run){
    if(run == "all"){
        return "B-H";
    }

    if(run.size() > 1){
        boost::to_upper(run);
        string temp = "";
        temp += run[0];
        temp += "-";
        temp += run.back();
        return  temp;
    }
    else{
        boost::to_upper(run);
        return run;
    }
}

extern double GetYLimit(TGraph* h1,TGraph* h2){

    double ymax;
    if(plt::GetYmax(h1) > plt::GetYmax(h2))
        ymax = plt::GetYmax(h1);
    else
        ymax = plt::GetYmax(h2);

    ymax *= 1.3;
    return ymax;
}

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

extern TH1D* SetComGraph(vector<double>& _bin, TGraph* dEff, TGraph* mEff, const string& method){
   
    double* bin    = &_bin[0];
    double  binnum = _bin.size();
    

    TH1D* ratio = new TH1D("ratio","",binnum-1,bin);
            
    for(int j=1;j<binnum;j++){
        double deff = dEff->GetY()[j-1];
        double meff = mEff->GetY()[j-1];
        double derr = dEff->GetErrorY(j-1);
        double merr = mEff->GetErrorY(j-1);
        double seff=0;
        double serr=0;

        if(meff!=0){
        
            if(method == "error")
                seff= (meff-deff)/deff;
            if(method == "compare")
                seff= deff/meff;
            
            serr = dra::ErrorProp(deff,derr,meff,merr);
           
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

extern void SetHist(TH1* h, const string& x, const string& y, const int& s){
    
    plt::SetAxis(h);
    h->GetXaxis()->SetTitle(x.c_str());
    h->GetYaxis()->SetTitle(y.c_str());
    h->GetXaxis()->SetLabelSize(s);

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
    pt->AddText(_text.c_str());
    pt->Draw();
    return pt;
}

extern TLegend* SetTLeg(const double& xmin,const double& ymin,const double& xmax,const double& ymax){
    
    TLegend* leg = plt::NewLegend(xmin,ymin,xmax,ymax);
    leg->SetLineColor(kWhite);
    leg->SetTextSize(14);
    leg->Draw();
    return leg;
}
