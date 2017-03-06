#include "TriggerEfficiency/TriggerDraw/interface/PlotObj.hpp"
#include "TriggerEfficiency/TriggerDraw/interface/eConstant.hpp"
#include "ManagerUtils/PlotUtils/interface/Common.hpp"
#include "TGraphAsymmErrors.h"
#include "TFile.h"
#include "TDirectory.h"
#include "TCanvas.h"
#include <iostream>
#include <fstream>
/*******************************************************************************
*   Global function
*******************************************************************************/
extern void CleanFile(){
    system( "rm temp.sh" );
    system( ( "rm " + trinamer.ResultsDir() / "data.root" ).c_str() );
    system( ( "rm " + trinamer.ResultsDir() / "mc.root"   ).c_str() );
}
extern void MergeFile(){
    SetRunFile();
    system( "chmod +x temp.sh" );
    system( "./temp.sh" );
}

extern void SetRunFile(){
    

    cout<<"start to merge"<<endl;
    string _datapath = SetDataPath();
    string mcpath   = SetMCPath();

    string run = trinamer.GetOption<string>("run");
    ofstream fout("temp.sh");
    string datacmd = "hadd " + trinamer.ResultsDir() / "data.root ";
    string mccmd   = "hadd " + trinamer.ResultsDir() / "mc.root   ";

    fout << "#!/bin/bash" << endl;
    
    string datapath="";
    if( run=="all"){
        datapath += (_datapath / "electron_"+"* ");
    }
    else{
        for(auto& r : run){
            datapath += (_datapath / "electron_"+r+"* " );
        }
    }

    cout<<"data path : "<<datapath<<endl;
    cout<<"mc   path : "<<mcpath / "electron*"<<endl;

    string dev = ">> /dev/null";
    fout<<(datacmd + datapath + dev)<<endl;
    fout<<(mccmd + mcpath ) / "electron*" + dev<<endl;

    fout.close();
}

extern string SetMCPath(){

    string package = "result_analyz";
    if (trinamer.CheckOption("method")){

        if(trinamer.GetOption<string>("method") == "changeMC")
            package+=  "_"+trinamer.GetOption<string>("method");
    
    }

    return ( trinamer.PackageDir() / "TriggerAnalyzer/data/MC_data/electron" / package );

}

extern string SetDataPath(){
    
    string package = "result_analyz";
    if (trinamer.CheckOption("method")){

        if(trinamer.GetOption<string>("method") != "changeMC")
            package+=  "_"+trinamer.GetOption<string>("method");
    
    }
    return ( trinamer.PackageDir() / "TriggerAnalyzer/data/electron" / package );
    
}

extern void PlotCompare(const string& tri){

    InitComMC(tri);
    InitComData(tri);
    
    SetGraphName(tri);
    
    PlotComparePt(tri) ;
    PlotCompareEta(tri);

    Clean();
}

extern void InitComData(const string& tri){ 

    TFile* file = TFile::Open( ( trinamer.GetSingleData<string>("datapath") ).c_str()  );
    TDirectory* dir = file->GetDirectory("demo");
    
    TH1D* passPt;
    TH1D* passEta;
    TH1D* totalPt;
    TH1D* totalEta;

    dir->GetObject( ("pass_pt_"  +tri).c_str(), passPt  );
    dir->GetObject( ("pass_eta_" +tri).c_str(), passEta );
    dir->GetObject( ("total_pt_" +tri).c_str(), totalPt );
    dir->GetObject( ("total_eta_"+tri).c_str(), totalEta);
    
    TGraphAsymmErrors* peff = new TGraphAsymmErrors() ; 
    TGraphAsymmErrors* eeff = new TGraphAsymmErrors();
    peff->BayesDivide(passPt,  totalPt,  "b");
    eeff->BayesDivide(passEta, totalEta,"b");
    
    trinamer.SetPtData( peff  );
    trinamer.SetEtaData( eeff  );

    file->Close();
}


extern void InitComMC(const string& tri){

    TFile* file = TFile::Open( ( trinamer.GetSingleData<string>("mcpath") ).c_str() );
    TDirectory* dir = file->GetDirectory("demo");
    
    TH1D* passPt;
    TH1D* passEta;
    TH1D* totalPt;
    TH1D* totalEta;

    dir->GetObject( ("pass_pt_"  +tri).c_str(),passPt);
    dir->GetObject( ("pass_eta_" +tri).c_str(),passEta);
    dir->GetObject( ("total_pt_" +tri).c_str(),totalPt);
    dir->GetObject( ("total_eta_"+tri).c_str(),totalEta);
    
    TGraphAsymmErrors* peff = new TGraphAsymmErrors() ; 
    TGraphAsymmErrors* eeff = new TGraphAsymmErrors();
    peff->BayesDivide(passPt,  totalPt, "b");
    eeff->BayesDivide(passEta, totalEta,"b");
    
    trinamer.SetPtMC( peff  );
    trinamer.SetEtaMC( eeff  );

    file->Close();
}


 
extern void PlotCompareEta(const string& tri){

    string triname = dra::GetSingle<string>("triname"    ,trinamer.GetSubTree(tri));
    string pcut    = dra::GetSingle<string>("pcut"       ,trinamer.GetSubTree(tri));
    vector<double> _ebin = dra::GetList<double>("eta" ,trinamer.GetSubTree(tri));
    SetGraph(trinamer.GetDataEta(), trinamer.GetMCEta());

    /******************************************************************************************/
    
    TCanvas* c = plt::NewCanvas();
    TPad* pad11= plt::NewTopPad();
    TPad* pad22= plt::NewBottomPad();
    pad11->Draw();
    pad22->Draw();

    /******************************************************************************************/
    
    pad11->cd();

    TH1F* h22=gPad->DrawFrame(eframe_x_min,eframe_y_min,eframe_x_max,eframe_y_max,"");
    SetHist(h22,"","Efficiency");
    ( trinamer.GetDataEta() )->Draw("EP same");
    ( trinamer.GetMCEta()   )->Draw("EP same");

    TLine* line = SetTLine(eline_x_min,eline_y_min,eline_x_max,eline_y_max);
    TLegend* leg = SetTLeg(eleg_x_min,eleg_y_min,eleg_x_max,eleg_y_max);
    leg->AddEntry( ("m"+tri).c_str(), ( "MC   "+triname+" ("+pcut+")" ).c_str(), "lp");
    leg->AddEntry( ("d"+tri).c_str(), ( "Data "+triname+" ("+pcut+")" ).c_str(), "lp");
    TPaveText* pave = SetTPave(etext_x_min,etext_y_min,etext_x_max,etext_y_max);
    plt::DrawCMSLabel();
    plt::DrawLuminosity(lumi);

    /******************************************************************************************/
    
    pad22->cd();
    TH1F* h33=gPad->DrawFrame(eframe_x_min,0.8,eframe_x_max,1.2,"");
    SetHist(h33,"Eta","Data / MC",10);
    h33->GetYaxis()->SetLabelSize( 12 );
    h33->GetXaxis()->SetTitleOffset( 3 );
    TH1D* ratio = SetComGraph(_ebin, trinamer.GetDataEta(), trinamer.GetMCEta(),"compare");
    ratio->Draw("EP same");

    TLine* line1 = SetTLine(eline_x_min,eline_y_min,eline_x_max,eline_y_max);
    
    /******************************************************************************************/
    plt::SaveToPDF( c, trinamer.GetFileName( "eff_eta_"+tri, "pdf" ) );
    SaveToRoot(ratio, trinamer.GetFileName("","root")  , "eta_"+tri );

    delete pad11;
    delete pad22;
    delete line;
    delete line1;
    delete leg;
    delete pave;
    delete c;
    delete ratio;
}

extern void PlotComparePt(const string& tri){
    

    //http://einverne.github.io/post/2016/01/boost-learning-note-7.html

    string triname = dra::GetSingle<string>("triname"    ,trinamer.GetSubTree(tri));
    string ecut    = dra::GetSingle<string>("ecut"       ,trinamer.GetSubTree(tri));
    vector<double> _pbin = dra::GetList<double>("pt" ,trinamer.GetSubTree(tri));
    
    SetGraph(trinamer.GetDataPt(), trinamer.GetMCPt());

    /******************************************************************************************/
    
    TCanvas* c = plt::NewCanvas();
    TPad* pad11= plt::NewTopPad();
    TPad* pad22= plt::NewBottomPad();
    pad11->Draw();
    pad22->Draw();

    /******************************************************************************************/
    
    pad11->cd();

    TH1F* h22=gPad->DrawFrame(pframe_x_min,pframe_y_min,pframe_x_max,pframe_y_max,"");
    plt::SetAxis(h22);
    SetHist(h22,"","Efficiency");
    ( trinamer.GetDataPt() )->Draw("EP same");
    ( trinamer.GetMCPt()   )->Draw("EP same");

    TLine* line = SetTLine(pline_x_min,pline_y_min,pline_x_max,pline_y_max);
    TLegend* leg = SetTLeg(pleg_x_min,pleg_y_min,pleg_x_max,pleg_y_max);
    leg->AddEntry( ("m"+tri).c_str(), ( "MC   "+triname+" ("+ecut+")" ).c_str(), "lp");
    leg->AddEntry( ("d"+tri).c_str(), ( "Data "+triname+" ("+ecut+")" ).c_str(), "lp");
    TPaveText* pave = SetTPave(ptext_x_min,ptext_y_min,ptext_x_max,ptext_y_max);
    plt::DrawCMSLabel();
    plt::DrawLuminosity(lumi);

    /******************************************************************************************/

    pad22->cd();
    TH1F* h33=gPad->DrawFrame(pframe_x_min,0.8,pframe_x_max,1.2,"");
    SetHist(h33,"Pt [GeV]","Data / MC",10);
    h33->GetYaxis()->SetLabelSize( 12 );
    h33->GetXaxis()->SetTitleOffset( 3 );
    TH1D* ratio = SetComGraph(_pbin, trinamer.GetDataPt(), trinamer.GetMCPt(),"compare");
    ratio->Draw("EP same");

    TLine* line1 = SetTLine(pline_x_min,pline_y_min,pline_x_max,pline_y_max);
    
    /******************************************************************************************/
    plt::SaveToPDF( c, trinamer.GetFileName( "eff_pt_"+tri, "pdf" ) );
    SaveToRoot(ratio, trinamer.GetFileName("","root")  , "pt_"+tri );

    delete pad11;
    delete pad22;
    delete line;
    delete line1;
    delete leg;
    delete pave;
    delete c;
    delete ratio;
}
