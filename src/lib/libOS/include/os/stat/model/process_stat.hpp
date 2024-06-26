#ifndef LIBOS_STAT_MODEL_PROCESS_STAT_HPP
#define LIBOS_STAT_MODEL_PROCESS_STAT_HPP

#include <wait.h>

#include "process_cpu_stat.hpp"
#include "process_state.hpp"

namespace OS
{
    namespace Stat
    {
        namespace Model
        {
            class ProcessStat
            {
            public:
                pid_t pid;
                ProcessCpuStat cpu_stat;
                ProcessState state;
                long threads;

                long priority;
                long nice;

                float mem_vsize;
                float mem_rss;
                float mem_pages_swapped;
                int processor;

            };
        }
    }
}

#endif