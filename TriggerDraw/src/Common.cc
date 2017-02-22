#include "TriggerEfficiency/TriggerDraw/interface/Common.hpp"
using namespace std;
using namespace boost::property_tree;





void dra::ReadFile(const string& file, ptree& root){
    read_json(file,root);
}



/*
string dra::GetSingle(const string& tag, const ptree& root ){
    return root.get<string>(tag);
}

vector<string> dra::GetList(const string& tag, const ptree& root){
    vector<string> ans;
    for (auto& n : root.get_child(tag)){
        ans.push_back(n.second.get_value<string>());
    }
    return ans;
}*/
