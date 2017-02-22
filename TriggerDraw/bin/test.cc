#include <string>
#include <vector>
#include <iostream>
#include "TriggerEfficiency/TriggerDraw/interface/Readmgr.hpp"

using namespace std;
namespace pt = boost::property_tree;


/*
class Readmgr{
    public:
        Readmgr(string file){
            dra::ReadFile(file,root);
        }

        ~Readmgr(){}
        
        boost::property_tree::ptree GetRoot(){
            return root;
        }

        template<typename T>
        T GetSingleData(const string& tag){
            return dra::GetSingle<T>(tag, root); 
        }

        template<typename T>
        vector<T> GetListData(const string& tag){
            return dra::GetList<T>(tag, root);
        }

    private:
        pt::ptree root;
};
*/



int main(){

    dra::Readmgr cfg("/wk_cms/sam7k9621/CMSSW_8_0_10/src/TriggerEfficiency/TriggerDraw/settings/test.json");
    string path = cfg.GetSingleData<string>("path");
    cout<<path<<endl;
    
}
