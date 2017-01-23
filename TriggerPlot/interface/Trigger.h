#ifndef PLOT_Trigger_HPP
#define PLOT_Trigger_HPP


#include "TFile.h"
#include "TH1.h"
#include "TDirectory.h"
#include "TGraphAsymmErrors.h"
#include <string>
using namespace std;
class hist{
	friend class Trigger;
    public:
    hist(string str);
	~hist(){delete f;}


    TH1D* getp(){
        return pass;
    }

    TH1D* gett(){
        return total;
    }

	private:
	TH1D* pass;
	TH1D* total;
	TFile *f;
	string p="pass_";
	string t="total_";
};

class Trigger{
	public:
		Trigger(string str);
		~Trigger(){
			delete eff_eta;
			delete eff_pt;
        }

		void setPt();
		void setEta();
		TGraphAsymmErrors* getp(){return eff_pt;}
		TGraphAsymmErrors* gete(){return eff_eta;}
		void draweff(string);


	private:
		string name;
		TGraphAsymmErrors* eff_eta;
		TGraphAsymmErrors* eff_pt;
		hist* pt;
		hist* eta;
};

class Triggermgr{
    public:
        Triggermgr(){}
        ~Triggermgr(){
            for(unsigned i =0;i<tris.size();i++){
                delete tris[i];
            }
        }
        void addtri(string,string,string);
        void drawpt();
        void draweta();

    private:
        vector<string> name;
        vector<string> pdes;
        vector<string> edes;
        vector<Trigger*> tris; 
        
};

#endif
