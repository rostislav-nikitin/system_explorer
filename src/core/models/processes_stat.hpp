#ifndef PROCESSES_STAT_HPP
#define PROCESSES_STAT_HPP

#include <map>
#include <wait.h>

#include "process_stat.hpp"
#include "processes_stat_common.hpp"

namespace SystemExplorer
{
    namespace Core
    {
        namespace Models
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