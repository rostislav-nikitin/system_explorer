#ifndef LIBOS_STAT_PARSER_PROC_PROCESS_STAT_PARSET_HPP
#define LIBOS_STAT_PARSER_PROC_PROCESS_STAT_PARSET_HPP

#include <string>
#include <istream>

#include "../model/proc_process_stat.hpp"

namespace OS
{
    namespace Stat
    {
        namespace Parser
        {
            class ProcProcessStatParser
            {
            public:
                Model::ProcProcessStat Parse(std::istream &stream);
            };
        }
    }
}

#endif