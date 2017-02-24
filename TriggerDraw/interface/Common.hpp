#ifndef COMMON_HPP
#define COMMON_HPP

//http://stackoverflow.com/questions/1433204/how-do-i-use-extern-to-share-variables-between-source-files-in-c

#include <string>
#include <vector>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
#include "TAttMarker.h"
#include "Rtypes.h"

/*******************************************************************************
*   Operator overloading, defined in src/Common.cc
*******************************************************************************/

extern std::string operator/( const std::string&, const std::string& );

/*******************************************************************************
*   Global variable, defined in src/Common.cc
*******************************************************************************/
extern Color_t color[10];
extern Style_t  mstyle[10];


namespace dra{
/*******************************************************************************
*   Common variable, defined in src/Common.cc
*******************************************************************************/




/*******************************************************************************
*   Common function, defined in src/Common.cc
*******************************************************************************/
extern void ReadFile(const std::string&  ,boost::property_tree::ptree&);

extern double ErrorProp(const double&,const double&, const double&, const double&);

extern std::string SubPackagePath(const std::string&, const std::string&);

extern std::string PackagePath(const std::string&);

/*******************************************************************************
*   Common template function, defined in src/Common.ipp
*******************************************************************************/
template<typename T>
extern T GetSingle(const std::string&, const boost::property_tree::ptree&);

template<typename T>
extern std::vector<T> GetList(const std::string&, const boost::property_tree::ptree&);
}

#include "TriggerEfficiency/TriggerDraw/src/Common.ipp"

#endif
