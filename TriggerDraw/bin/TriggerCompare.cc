#include <iostream>
#include <vector>
#include <math.h>


#include "ManagerUtils/PlotUtils/interface/Common.hpp"
#include "TriggerEfficiency/TriggerDraw/interface/Readmgr.hpp"
#include "TriggerEfficiency/TriggerDraw/interface/Parsermgr.hpp"
//#include "TriggerEfficiency/TriggerDraw/interface/Trigger.hpp"
#include "TriggerEfficiency/TriggerDraw/interface/eConstant.hpp"
#include "TriggerEfficiency/TriggerDraw/interface/Common.hpp"
#include "TriggerEfficiency/TriggerDraw/interface/PlotObj.hpp"


using namespace std;
namespace opt = boost::program_options;

int main(int argc, char* argv[]){

    opt::options_description de( "Command for TriggerCompare" );
    
    //http://stackoverflow.com/questions/8175723/vector-arguments-in-boost-program-options
    de.add_options()
        ( "lepton,l", opt::value<string>()->required(), "Compare which lepton" )
        ( "run,r", opt::value<string>()->required(), "Use which era of input" )
        ( "method,m", opt::value<string>()->required(), "Output file name" )
    ;

    trinamer.AddOptions( de );
    const int run = trinamer.ParseOptions( argc, argv );
    if( run == dra::Parsermgr::HELP_PARSER  ){ return 0; }
    if( run == dra::Parsermgr::FAIL_PARSER ){ return 1; }
    trinamer.SetFileName( {"lepton","method","run"}  );



    
    for( auto& tri : trinamer.GetListData<string>("triggerlist")  ){
        PlotCompare(tri);
    }

    PlotSysError("ele27");
}
