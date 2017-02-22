#ifndef READMGR_HPP
#define READMGR_HPP


#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <string>
#include <vector>

namespace dra{

    
    class Readmgr{
        public:
            Readmgr(const std::string&);

            ~Readmgr(){}
        
            boost::property_tree::ptree GetRoot();

            template<typename T>
            T GetSingleData(const std::string&);

            template<typename T>
            std::vector<T> GetListData(const std::string&);

        private:
            boost::property_tree::ptree root;
    
    };





}

#include "TriggerEfficiency/TriggerDraw/src/Readmgr.ipp"

#endif
