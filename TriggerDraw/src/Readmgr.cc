#include "TriggerEfficiency/TriggerDraw/interface/Readmgr.hpp"

#include "TriggerEfficiency/TriggerDraw/interface/Common.hpp"

using namespace std;
namespace pt = boost::property_tree;

dra::Readmgr::Readmgr(const string& file){
    dra::ReadFile(file,root);
}

pt::ptree
dra::Readmgr::GetSubTree(const string& t){
    return root.get_child(t);
} 

