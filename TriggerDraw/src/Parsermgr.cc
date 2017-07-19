#include "TriggerEfficiency/TriggerDraw/interface/Parsermgr.hpp"
#include <iostream>
#include <boost/exception/diagnostic_information.hpp>
#include <boost/lexical_cast.hpp>

namespace opt = boost::program_options;
using namespace std;


dra::Parsermgr::Parsermgr()
{
    desc.add_options()
    ( "help,h", "print help options and exit program" )
  ;
}

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
        cerr << desc <<endl;
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

void dra::Parsermgr::AddFileName(const string& opt){
    if(CheckOption(opt)){
        namelist.push_back(opt);
    }
}

string dra::Parsermgr::OptName(const string& opt){
    string ans = "";
    
    if( ans == "" ){
        try {
            ans = GetOption<string>( opt );
      } catch( ... ){
      }
    }

    if( ans == "" ){
        try {
            ans = boost::lexical_cast<string>( GetOption<int>( opt ) );
      } catch( ... ){
      }
    }

    if( ans == "" ){
        try {
            ans = boost::lexical_cast<string>( GetOption<double>( opt ) );
      } catch( ... ){
      }
    }

    return ans;


}








