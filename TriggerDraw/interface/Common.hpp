#ifndef COMMON_HPP
#define COMMON_HPP



#include <string>
#include <vector>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>



namespace dra{
/*******************************************************************************
*   Common variable, defined in src/Common.cc
*******************************************************************************/

/*******************************************************************************
*   Common function, defined in src/Common.cc
*******************************************************************************/
extern void ReadFile(const std::string&  ,boost::property_tree::ptree&);



/*******************************************************************************
*   Common template function, defined in src/Common.cc
*******************************************************************************/
    template<typename T>
    extern T GetSingle(const std::string&, const boost::property_tree::ptree&);

    template<typename T>
    extern std::vector<T> GetList(const std::string&, const boost::property_tree::ptree&);
}

#include "TriggerEfficiency/TriggerDraw/src/Common.ipp"

#endif
