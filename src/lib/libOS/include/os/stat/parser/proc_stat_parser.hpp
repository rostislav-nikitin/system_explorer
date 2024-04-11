#ifndef LIBOS_STAT_PARSER_PROC_STAT_PARSER_HPP
#define LIBOS_STAT_PARSER_PROC_STAT_PARSER_HPP

#include <istream>

#include "../model/proc_stat.hpp"

namespace OS
{
    namespace Stat
    {
        namespace Parser
        {
            class ProcStatParser
            {
            public:
                Model::ProcStat Parse(std::istream &stream);
            };
        }
    }
}

#endif