#ifndef LIBOS_STAT_MODEL_PROCESSES_STAT_HPP
#define LIBOS_STAT_MODEL_PROCESSES_STAT_HPP

#include <map>
#include <wait.h>

#include "process_stat.hpp"
#include "processes_stat_common.hpp"

namespace OS
{
    namespace Stat
    {
        namespace Model
        {
            class ProcessesStat
            {
            public:
                ProcessesStatCommon processes_stat_common;
                std::map<pid_t, ProcessStat> processes_stat;
            };
        }
    }
}

#endif