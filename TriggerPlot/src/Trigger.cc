#include "TCanvas.h"
#include "TAttMarker.h"
#include "TStyle.h"
#include "TDirectory.h"
#include "TLegend.h"
#include "TLatex.h"
#include "ManagerUtils/PlotUtils/interface/Common.hpp"
#include "TriggerEfficiency/TriggerPlot/interface/eConstant.h"
#include "TriggerEfficiency/TriggerPlot/interface/Trigger.h"
hist::hist(string str){
	f = new TFile(filename);

    TDirectory* dir1 = gFile->GetDirectory(dirname);
	p+=str;
    t+=str;
	dir1->GetObject(p.c_str(),pass);
	dir1->GetObject(t.c_str(),total);
}

Trigger::Trigger(string str){
	name=str;   //tr22
	eff_eta=0;
	eff_pt=0;

	pt=new hist("pt_"+name);
	eta=new hist("eta_"+name);

	setEta();
	setPt();
}
void Trigger::setPt(){   //eff_pt_
	eff_pt=new TGraphAsymmErrors(pt->pass,pt->total,"b");
	eff_pt->SetName( ("eff_pt_"+name).c_str() );
}
void Trigger::setEta(){   //eff_eta_
	eff_eta=new TGraphAsymmErrors(eta->pass,eta->total,"b");
	eff_eta->SetName( ("eff_eta_"+name).c_str());
}
void Trigger::draweff(string str2){    //eff_pt_tr50 /trigger name
	gStyle->SetOptStat(0);
	TCanvas* c1=new TCanvas("c1","c1");
    TH1F* h2=gPad->DrawFrame(pframe_x_min,pframe_y_min,pframe_x_max,pframe_y_max,"");
    plt::SetAxis(h2);
    h2->GetXaxis()->SetTitle("P_{T} [GeV/c^{2}]");
    h2->GetYaxis()->SetTitle("Efficiency");
    eff_pt->Draw("EP");


	eff_pt->SetMarkerStyle(21);
    eff_pt->SetMarkerSize(0.6);
	eff_pt->SetLineColor(kRed+2);
	eff_pt->SetMarkerColor(kRed+2);

    TLegend *leg1 = plt::NewLegend(pleg_x_min,pleg_y_min,pleg_x_max,pleg_y_max);
    leg1->SetLineColor(kWhite);
	leg1->AddEntry( ("eff_pt_"+name).c_str(),str2.c_str(),"lp");
	leg1->Draw();

    plt::DrawCMSLabel();
    plt::DrawLuminosity(lumi);

    TLine *line = new TLine(pline_x_min,pline_y_min,pline_x_max,pline_y_max);
    line->SetLineColor(kRed);
    line->SetLineStyle(8);
    line->Draw();

    TPaveText *pt = plt::NewTextBox(ptext_x_min,ptext_y_min,ptext_x_max,ptext_y_max);
    pt->AddText(text);
    pt->Draw();
    plt::SaveToPDF(c1, ("eff_pt_"+name+".pdf").c_str() );
	delete leg1;
	delete c1;
}

void Triggermgr::addtri(string n,string s1,string s2){
    name.push_back(n);
    tris.push_back(new Trigger(n));
    pdes.push_back(s1);
    edes.push_back(s2);
}
void Triggermgr::drawpt(){
    for(unsigned i=0;i<tris.size();i++){
        tris[i]->draweff(pdes[i]);
    }
}

void Triggermgr::draweta(){

    extern Color_t color[10];
    extern Style_t mstyle[10];

     TCanvas* c2=new TCanvas("c2","c2");
     TH1F* h2=gPad->DrawFrame(eframe_x_min,eframe_y_min,eframe_x_max,eframe_y_max,"");
     plt::SetAxis(h2);h2->GetXaxis()->SetTitle("Eta");
     h2->GetYaxis()->SetTitle("Efficiency");

     for(unsigned i=0;i<tris.size();i++){
         tris[i]->gete()->Draw("EP same");
         tris[i]->gete()->SetLineColor(color[i]);
         tris[i]->gete()->SetMarkerColor(color[i]);
         tris[i]->gete()->SetMarkerStyle(mstyle[i]);
         tris[i]->gete()->SetMarkerSize(0.6);
     }
     TLine *line = new TLine(eline_x_min,eline_y_min,eline_x_max,eline_y_max);
     line->SetLineColor(kRed);
     line->SetLineStyle(8);
     line->Draw();

     TPaveText *pt = plt::NewTextBox(etext_x_min,etext_y_min,etext_x_max,etext_y_max);
     pt->AddText(text);
     pt->Draw();
     plt::DrawCMSLabel();
     plt::DrawLuminosity(lumi);

     TLegend *leg = plt::NewLegend(eleg_x_min,eleg_y_min,eleg_x_max,eleg_y_max);
     leg->SetLineColor(kWhite);


     for(unsigned i=0;i<tris.size();i++){
         leg->AddEntry( ("eff_eta_"+name[i]).c_str() ,edes[i].c_str(),"lp");
     }
     leg->Draw();
     plt::SaveToPDF(c2,etapdf);


}
