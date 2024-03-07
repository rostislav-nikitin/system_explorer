#ifndef PROC_PROCESS_STAT_PARSET_HPP
#define PROC_PROCESS_STAT_PARSET_HPP

#include <string>
#include <istream>

#include "../models/proc_process_stat.hpp"

namespace SystemExplorer
{
    namespace Core
    {
        namespace Parsers
        {
            class ProcProcessStatParser
            {
            public:
                Models::ProcProcessStat Parse(std::istream &stream);
            };
        }
    }
}

#endif