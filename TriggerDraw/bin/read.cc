
#include <iostream>
#include <vector>
#include <math.h>

#include "ManagerUtils/PlotUtils/interface/Common.hpp"
#include "TriggerEfficiency/TriggerDraw/interface/Readmgr.hpp"
#include "TriggerEfficiency/TriggerDraw/interface/Parsermgr.hpp"
#include "TriggerEfficiency/TriggerDraw/interface/Trigger.hpp"
#include "TriggerEfficiency/TriggerDraw/interface/eConstant.hpp"
#include "TriggerEfficiency/TriggerDraw/interface/Common.hpp"

using namespace std;
namespace opt = boost::program_options;
namespace pt = boost::property_tree;

int main(){
    dra::Readmgr cfg("/wk_cms/sam7k9621/CMSSW_8_0_10/src/TriggerEfficiency/TriggerDraw/settings/eTrigger.json");
    

    vector<double> temp;

    for(auto& cut : (cfg.GetSubTree("ele27")).get_child("pt")){
        temp.push_back(cut.second.get_value<double>());
    }

    for(auto& t : temp){
        cout<<t<<endl;
    }







}

