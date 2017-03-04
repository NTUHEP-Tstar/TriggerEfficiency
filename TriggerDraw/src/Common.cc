#include "TriggerEfficiency/TriggerDraw/interface/Common.hpp"
#include <stdlib.h>
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

string dra::SubPackagePath(const string& package, const string& sub){
    return dra::PackagePath(package) / sub;
}

//https://ubuntuforums.org/archive/index.php/t-895400.html
string dra::PackagePath(const string& package){
    string base = getenv("CMSSW_BASE");
    return ( base / "src") / package;
}






