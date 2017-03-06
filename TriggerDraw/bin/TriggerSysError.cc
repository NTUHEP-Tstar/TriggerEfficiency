#include <iostream>
#include <vector>
#include <math.h>

#include "ManagerUtils/PlotUtils/interface/Common.hpp"
#include "TriggerEfficiency/TriggerDraw/interface/Readmgr.hpp"
#include "TriggerEfficiency/TriggerDraw/interface/Parsermgr.hpp"
#include "TriggerEfficiency/TriggerDraw/interface/eConstant.hpp"
#include "TriggerEfficiency/TriggerDraw/interface/Common.hpp"
#include "TriggerEfficiency/TriggerDraw/interface/PlotObj.hpp"

using namespace std;
namespace opt = boost::program_options;


int main(int argc, char* argv[]){

    opt::options_description de( "Command for TriggerCompare" );
    
    //http://stackoverflow.com/questions/8175723/vector-arguments-in-boost-program-options
    de.add_options()
        ( "lepton,l", opt::value<string>()->required(), "Which lepton " )
        ( "run,r", opt::value<string>()->required(), "Which part of era " )
        ( "method,m", opt::value<string>()->required(), "Which systematic variable " )
    ;

    trinamer.AddOptions( de );
    const int run = trinamer.ParseOptions( argc, argv );
    if( run == dra::Parsermgr::HELP_PARSER  ){ return 0; }
    if( run == dra::Parsermgr::FAIL_PARSER ){ return 1; }
    trinamer.SetFileName( {"lepton","run","method"} );


    CalcLumi();

    cout<<_text<<endl;
    cout<<"Total lumi : "<<lumi<<endl;

    for( auto& tri : trinamer.GetListData<string>("triggerlist")  ){
        PlotSysError(tri);
    }
   
}
