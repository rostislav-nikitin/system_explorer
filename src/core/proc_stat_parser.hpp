#include <istream>

#include "proc_stat.hpp"

namespace SystemExplorer
{
    namespace Core
    {
        class ProcStatParser
        {
        public:
            ProcStat Parse(std::istream &stream);
        };
    }
}