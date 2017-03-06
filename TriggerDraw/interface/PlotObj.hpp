#include "TriggerEfficiency/TriggerDraw/interface/Triggermgr.hpp"
#include "TAttMarker.h"
#include "Rtypes.h"
#include "TLine.h"
#include "TLegend.h"
#include "TPaveText.h"
#include <string>
#include <vector>
#include "TH1.h"
#include "TFile.h"
/*******************************************************************************
*   Global variables
*******************************************************************************/

extern dra::Triggermgr trinamer;
extern TFile f;
extern double lumi;
extern string _text;

extern Color_t  color[10];
extern Style_t  mstyle[10];

/*******************************************************************************
*   Global function
*******************************************************************************/
/**********************
*   CommonPlot        *
**********************/
extern string SetEra(string);
extern void CalcLumi();
extern void SaveToRoot(TH1*, const string&, const string&);
extern void Clean();
extern void SetGraphName(const string&);
extern void SetGraph(TGraph*, TGraph*);
extern void SetHist(TH1*,const string&, const string&, const int& =0);
extern TH1D* SetComGraph(vector<double>&, TGraph*, TGraph*,const string&);
extern TLine* SetTLine(const double&,const double&,const double&,const double&);
extern TPaveText* SetTPave(const double&,const double&,const double&,const double&);
extern TLegend* SetTLeg(
        const double&,
        const double&,
        const double&,
        const double&
        );

extern double GetYLimit(TGraph*,TGraph*);

/**********************
*   TriCompare        *
**********************/
extern void CleanFile();
extern void MergeFile();
extern string SetDataPath();
extern string SetMCPath();
extern void SetRunFile();
extern void PlotCompare(const string&);
extern void InitComData(const string&);
extern void InitComMC(const string&);
extern void PlotComparePt(const string&);
extern void PlotCompareEta(const string&);
extern void PlotCompare(const string&);


/**********************
*   TriSysError       *
**********************/
extern void SetPath(string&,const string& ="");
extern void PlotSysError(const string&);
extern void InitSysOri(const string&);
extern void InitSysNew(const string&);
extern void PlotSysErrorPt(const string&);
extern void PlotSysErrorEta(const string&);
