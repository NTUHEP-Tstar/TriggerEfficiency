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

extern Color_t  color[10];
extern Style_t  mstyle[10];

/*******************************************************************************
*   Global function
*******************************************************************************/
extern void SaveToRoot(TH1*, const string&, const string&);

extern void PlotCompare(const string&);
extern void PlotPt(const string&);
extern void PlotEta(const string&);
extern void InitComData(const string&);
extern void InitComMC(const string&);
extern void Clean();

extern void SetGraphName(const string&);
extern void PlotCompare(const string&);
extern void SetGraph(TGraph*, TGraph*);
extern TH1D* SetComGraph(vector<double>&, TGraph*, TGraph*);

extern TLine* SetTLine(const double&,const double&,const double&,const double&);
extern TPaveText* SetTPave(const double&,const double&,const double&,const double&);
extern TLegend* SetTLeg(
        const string&,
        const string&,
        const string&,
        const double&,
        const double&,
        const double&,
        const double&
        );


extern void PlotSysError(const string&);
extern void InitSysOri(const string&);
extern void InitSysNew(const string&);
