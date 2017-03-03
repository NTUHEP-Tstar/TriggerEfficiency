#ifndef TRIGGERMGR_HPP
#define TRIGGERMGR_HPP

#include "TEfficiency.h"
#include "TGraphAsymmErrors.h"
#include "TH1.h"
#include "TFile.h"
#include <string>
#include <vector>

#include "TriggerEfficiency/TriggerDraw/interface/Common.hpp"
#include "TriggerEfficiency/TriggerDraw/interface/Readmgr.hpp"
#include "TriggerEfficiency/TriggerDraw/interface/Pathmgr.hpp"

namespace dra{

    class Triggermgr : dra::Pathmgr, public dra::Readmgr
    {
        public :
            Triggermgr(const string&, const string&, const string&);
            ~Triggermgr();

            TGraphAsymmErrors* getPtEff();
            TGraphAsymmErrors* getEtaEff();


        private:
            vector<string> triggerlist;
            vector<string> triggername;
            vector<string> pcut;
            vector<string> ecut;

            TH1D* passPt;
            TH1D* passEta;
            TH1D* totalPt;
            TH1D* totalEta;
            TGraphAsymmErrors* effPt;
            TGraphAsymmErrors* effEta;


            TFile* File;
    };


}

#endif
