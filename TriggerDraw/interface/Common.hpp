#ifndef COMMON_HPP
#define COMMON_HPP



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
