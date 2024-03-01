#include <string>
#include <istream>

#include "proc_process_stat.hpp"

namespace SystemExplorer
{
    namespace Core
    {
        class ProcProcessStatParser
        {
        public:
            ProcProcessStat Parse(std::istream &stream);
        };
    }
}