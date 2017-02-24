#ifndef PARSERMGR_IPP
#define PARSERMGR_IPP

#include "TriggerEfficiency/TriggerDraw/interface/Parsermgr.hpp"
 

namespace opt = boost::program_options;
using namespace std;

template <typename T>
T dra::Parsermgr::GetOption(const string& op){
    return vm[op].as<T>();
}


//http://stackoverflow.com/questions/1657883/variable-number-of-arguments-in-c
template <typename T>
void dra::Parsermgr::SetFileName(initializer_list<T> list){
    for(auto name : list){
        namelist.push_back(name);
    }
}




#endif
