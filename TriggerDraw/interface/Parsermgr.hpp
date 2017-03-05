#ifndef PARSERMGR_HPP
#define PARSERMGR_HPP

#include "TriggerEfficiency/TriggerDraw/interface/Common.hpp"
#include "TriggerEfficiency/TriggerDraw/interface/Readmgr.hpp"
#include "TriggerEfficiency/TriggerDraw/interface/Pathmgr.hpp"
#include <boost/program_options.hpp>
#include <initializer_list>

namespace dra{

    class Parsermgr 
    {

        public:
            enum PARSING_STATUS{
                FAIL_PARSER,
                HELP_PARSER,
                PASS_PARSER
            };
            
            Parsermgr();
            ~Parsermgr(){}

            Parsermgr& AddOptions(const boost::program_options::options_description&);
            int ParseOptions(int argc, char* argv[]);
            bool CheckOption(const std::string&);
           
            string OptName(const std::string&);
            std::vector<std::string> GetNamelist(){
                return namelist;
            }

            void AddFileName(const std::string&);

            template <typename T>
            void SetFileName(std::initializer_list<T>);

            template <typename T>
            T GetOption(const std::string&);

        private:
            boost::program_options::variables_map vm;
            boost::program_options::options_description desc;
            std::vector<std::string> namelist;
    };

}

#include "TriggerEfficiency/TriggerDraw/src/Parsermgr.ipp"

#endif
