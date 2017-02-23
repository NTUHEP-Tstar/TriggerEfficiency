#include "TriggerEfficiency/TriggerDraw/interface/Common.hpp"
using namespace std;
using namespace boost::property_tree;





void dra::ReadFile(const string& file, ptree& root){
    read_json(file,root);
}



