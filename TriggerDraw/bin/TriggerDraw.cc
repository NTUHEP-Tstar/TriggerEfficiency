#include <iostream>
#include <string>
#include <vector>
#include <boost/exception/diagnostic_information.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/program_options.hpp>
#include "TriggerEfficiency/TriggerDraw/interface/Common.hpp"

namespace opt = boost::program_options;
using namespace std;

template<typename T>
T GetInput( string op, opt::variables_map vm  )  {
    return vm[op].as<T>();
}

template<typename T>
vector<T> GetInputList(string op, opt::variables_map vm) {
    return vm[op].as<vector<T>>();
}


int main( int argc, char* argv[] )
{


    opt::options_description desc( "Allowed options" );

    desc.add_options()
    ("help", "produce help message")
    ("lepton,l", opt::value<int>(), "set lepton")
    ;
    opt::variables_map vm;

    try {
        opt::store( opt::parse_command_line( argc, argv, desc ), vm );
        opt::notify( vm );
    } catch( boost::exception& e ){
        cerr << "Error parsing command!" << endl;
        cerr << boost::diagnostic_information( e );
        cerr << desc << endl;
    }

    if (vm.count("help"))
    {
        cout << desc << "\n";
    }




    string lepton="";
    
    if(lepton == ""){
        try {
            lepton = GetInput<string>("lepton",vm) ;
        } catch( ...  ){
        }
    }
    
    cout<<"pass first try: "<<lepton<<endl;

    if(lepton == ""){
        try {
            lepton = boost::lexical_cast<string> ( GetInput<int>("lepton",vm) );
        } catch( ...  ){
        }
    }

    cout<<"pass second try: "<<lepton<<endl;
/*   if(lepton == "electron"){
       for(auto& lep : elsource){
           cout<<lep<<endl;
       }
   }
   else if(lepton == "muon"){
       for(auto& lep : musource){
           cout<<lep<<endl;
       }
   }
   else
       cerr<<"wrong lepton input"<<endl;
*/

}
