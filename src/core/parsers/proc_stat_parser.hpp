#ifndef PROC_STAT_PARSER_HPP
#define PROC_STAT_PARSER_HPP

#include <istream>

#include "../models/proc_stat.hpp"

namespace SystemExplorer
{
    namespace Core
    {
        namespace Parsers
        {
            class ProcStatParser
            {
            public:
                Models::ProcStat Parse(std::istream &stream);
            };
        }
    }
}

#endif