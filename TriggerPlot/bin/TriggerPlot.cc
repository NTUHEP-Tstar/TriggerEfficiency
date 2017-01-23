#include "ManagerUtils/PlotUtils/interface/Common.hpp"
#include "TriggerEfficiency/TriggerPlot/interface/Trigger.h"
using namespace std;
int main(){

    Triggermgr mgr;
/*    mgr.addtri(
            "total_mu27_tkmu27",
            "IsoMu27 || IsoTkMu27",
            "IsoMu27 || IsoTkMu27 (p_{T} > 27 GeV)"
            );
    mgr.addtri(
            "toal_mu24_tkmu24",
            "IsoMu24 || IsoTkMu24",
            "IsoMu24 || IsoTkMu24 (p_{T} > 24 GeV)"
            );
    mgr.addtri(
            "total_mu45",
            "Mu45_eta2p1  (| #eta | < 2.1)     ",
            "Mu45_eta2p1  (p_{T} > 48 GeV)      "
            );
    mgr.addtri(
            "total_mu50_tkmu50",
            "Mu50 || TkMu50",
            "Mu50 || TkMu50 (p_{T} > 53 GeV)"
            );
    mgr.addtri(
            "total_mu22_tkmu22",
            "IsoMu22 || IsoTkMu22",
            "IsoMu22 || IsoTkMu22 (p_{T} > 22 GeV)"
            );
*/
    
    mgr.addtri(
            "ele27",
            "Ele45_WPLoose* || Ele27_eta2p1_WPLoost (| #eta | < 2.1)",
            "Ele45_WPLoose* || Ele27_eta2p1_WPLoost (p_{T} > 48 GeV)"
            );
    mgr.addtri(
            "ele32",
            "Ele32_eta2p1_WPTight (| #eta | < 2.1)",
            "Ele32_eta2p1_WPTight (p_{T} > 35 GeV)"
            );
    mgr.drawpt();
    mgr.draweta();


}
