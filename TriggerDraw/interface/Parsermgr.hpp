#ifndef PARSERMGR_HPP
#define PARSERMGR_HPP

#include "TriggerEfficiency/TriggerDraw/interface/Common.hpp"
#include <boost/program_options.hpp>

namespace dra{

    class Parsermgr{

        public:
            enum PARSING_STATUS{
                FAIL_PARSER,
                HELP_PARSER,
                PASS_PARSER
            };


            Parsermgr& AddOptions(const boost::program_options::options_description&);
            int ParseOptions(int argc, char* argv[]);
            bool CheckOption(const std::string&);

            template <typename T>
            T GetOption(const std::string&);

        private:
            boost::program_options::variables_map vm;
            boost::program_options::options_description desc;
    };

}

#include "TriggerEfficiency/TriggerDraw/src/Parsermgr.ipp"

#endif
