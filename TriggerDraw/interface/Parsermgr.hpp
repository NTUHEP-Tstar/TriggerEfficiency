#ifndef PARSERMGR_HPP
#define PARSERMGR_HPP

#include "TriggerEfficiency/TriggerDraw/interface/Common.hpp"
#include "TriggerEfficiency/TriggerDraw/interface/Readmgr.hpp"
#include "TriggerEfficiency/TriggerDraw/interface/Pathmgr.hpp"
#include <boost/program_options.hpp>
#include <initializer_list>

namespace dra{

    class Parsermgr : public dra::Pathmgr, public dra::Readmgr
    {

        public:
            enum PARSING_STATUS{
                FAIL_PARSER,
                HELP_PARSER,
                PASS_PARSER
            };
            
            Parsermgr(const std::string&);
            ~Parsermgr(){}

            Parsermgr& AddOptions(const boost::program_options::options_description&);
            int ParseOptions(int argc, char* argv[]);
            bool CheckOption(const std::string&);
           
            string GetFileName(const std::string&, const std::string&);
            string OptName(const std::string&);
            

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
