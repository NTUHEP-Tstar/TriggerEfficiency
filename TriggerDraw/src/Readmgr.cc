#include "TriggerEfficiency/TriggerDraw/interface/Readmgr.hpp"

#include "TriggerEfficiency/TriggerDraw/interface/Common.hpp"

using namespace std;
namespace pt = boost::property_tree;

dra::Readmgr::Readmgr(const string& file){
    dra::ReadFile(file,root);
}

pt::ptree
dra::Readmgr::GetRoot(){
    return root;
}


/*
template<typename T>
T dra::Readmgr::GetSingleData(const string& tag){
    return dra::GetSingle<T>(tag,root);
}


template<typename T>
vector<T> dra::Readmgr::GetListData(const string& tag){
    return dra::GetList<T>(tag, root);
}

*/
