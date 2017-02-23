#include <string>
#include <vector>
#include <iostream>
#include "TriggerEfficiency/TriggerDraw/interface/Readmgr.hpp"
#include "TriggerEfficiency/TriggerDraw/interface/Parsermgr.hpp"
using namespace std;
namespace pt = boost::property_tree;


int main(int argc, char* argv[]){

    opt::options_description command_read_json( "Options for reading json file" );
    command_read_json.add_options()
    ( "path,p"  , opt::value<string>(), "show which path" )
    ( "lepton,l", opt::value<string>(), "show which lepton" )
    ;

    opt::options_description command_test_input("Options for testing input");
    command_test_input.add_options()
    ( "test,t"  , opt::value<double>(), "show the testing input")
    ;

    dra::Parsermgr trinamer;
    trinamer.AddOptions( command_read_json ).AddOptions( command_test_input );
    const int run = trinamer.ParseOptions( argc, argv );
    if( run == dra::Parsermgr::HELP_PARSER  ){ return 0; }
    if( run == dra::Parsermgr::FAIL_PARSER ){ return 1; }


    string lepton="";
    if(trinamer.CheckOption("lepton")){
        lepton = trinamer.GetOption<string>("lepton");
    }

    double a = 0;
    if(trinamer.CheckOption("test")){
        a += trinamer.GetOption<double>("test");
    }
    
    if(lepton == "sam7k9621"){
        dra::Readmgr cfg("/wk_cms/sam7k9621/CMSSW_8_0_10/src/TriggerEfficiency/TriggerDraw/settings/test.json");
        string path = cfg.GetSingleData<string>("path");
        cout<<path<<endl;
        cout<<a<<endl;
    }
    else{
        cout<<"wrong account"<<endl;
        cout<<a<<endl;
    }
}
