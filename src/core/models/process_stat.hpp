#ifndef PROCESS_STAT_HPP
#define PROCESS_STAT_HPP

#include <wait.h>

#include "process_cpu_stat.hpp"

namespace SystemExplorer
{
    namespace Core
    {
        namespace Models
        {
            class ProcessStat
            {
            public:
                pid_t pid;
                ProcessCpuStat cpu_stat;
            };
        }
    }
}

#endif