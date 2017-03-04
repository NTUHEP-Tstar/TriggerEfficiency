#ifndef TRIGGERMGR_HPP
#define TRIGGERMGR_HPP

#include <string>
#include <vector>

#include "TGraph.h"

#include "TriggerEfficiency/TriggerDraw/interface/Common.hpp"
#include "TriggerEfficiency/TriggerDraw/interface/Readmgr.hpp"
#include "TriggerEfficiency/TriggerDraw/interface/Pathmgr.hpp"
#include "TriggerEfficiency/TriggerDraw/interface/Parsermgr.hpp"

namespace dra{

    class Triggermgr : dra::Pathmgr, public dra::Readmgr, public dra::Parsermgr
    {
        public :
            Triggermgr(const string&);
            ~Triggermgr();

            string GetFileName(const string&, const string&);

            void SetPtData (TGraph* );
            void SetEtaData(TGraph* );
            void SetPtMC   (TGraph* );
            void SetEtaMC  (TGraph* );

            TGraph* GetDataPt();
            TGraph* GetDataEta();
            TGraph* GetMCPt();
            TGraph* GetMCEta();

        private:

            TGraph* datapt; 
            TGraph* dataeta; 
            TGraph* mcpt; 
            TGraph* mceta; 


            

    };




}

#endif
