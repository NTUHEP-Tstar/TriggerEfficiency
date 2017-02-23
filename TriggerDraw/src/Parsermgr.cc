#include "TriggerEfficiency/TriggerDraw/interface/Parsermgr.hpp"
#include <boost/exception/diagnostic_information.hpp>

namespace opt = boost::program_options;
using namespace std;

dra::Parsermgr& dra::Parsermgr::AddOptions(const opt::options_description& de){
    desc.add(de);
    return *this;
}

int dra::Parsermgr::ParseOptions(int argc, char* argv[]){


    try {
        opt::store( opt::parse_command_line( argc, argv, desc ), vm );
        opt::notify( vm );
    } catch( boost::exception& e ){
        cerr << "Error parsing command!" << endl;
        cerr << boost::diagnostic_information( e );
        return FAIL_PARSER;
    }

    if (vm.count("help")){
        cout<<desc<<endl;
        return HELP_PARSER;
    }
    
    return PASS_PARSER;


}

bool dra::Parsermgr::CheckOption(const string& option){
    return vm.count(option);    
}


