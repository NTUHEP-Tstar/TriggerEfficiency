#include "TriggerEfficiency/TriggerDraw/interface/Common.hpp"
using namespace std;
using namespace boost::property_tree;

/*******************************************************************************
*   Operator overloading
*******************************************************************************/

string operator/( const string& x , const string& y )
{
  return x+"/"+y;
}


/*******************************************************************************
*   Global variable
*******************************************************************************/
Color_t color[10] = {kRed+2,kBlue-7,kGreen+3,kMagenta+2,kYellow-3,kGreen-9,kBlue-9,kRed-9,kCyan+3,kGray };

Style_t  mstyle[10] = {20,21,24,25,31,33,27,23,22,26};


/*******************************************************************************
*   Common function
*******************************************************************************/

void dra::ReadFile(const string& file, ptree& root){
    read_json(file,root);
}

double dra::ErrorProp(const double& x1, const double& m1, const double & x2, const double& m2){
    double result = sqrt( (m1*m1)/(x2*x2) + (m2*m2)*(x1*x1)/(x2*x2*x2*x2));
    return result;
}

