#ifndef TRIGGER_HPP
#define TRIGGER_HPP

#include "TEfficiency.h"
#include "TGraphAsymmErrors.h"
#include "TH1.h"
#include <string>


namespace dra{
    
    class Trigger{
    public:
        Trigger(const std::string& ,bool );
        ~Trigger(){
        }

        TGraphAsymmErrors* getPtEff();
        TGraphAsymmErrors* getEtaEff();

    private:
        TH1D* passPt;
        TH1D* passEta;
        TH1D* totalPt;
        TH1D* totalEta;
        TGraphAsymmErrors* effPt;
        TGraphAsymmErrors* effEta;

    };
}

#endif
