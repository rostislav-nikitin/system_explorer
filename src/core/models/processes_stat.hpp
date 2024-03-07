#ifndef PROCESSES_STAT_HPP
#define PROCESSES_STAT_HPP

#include <map>
#include <wait.h>

#include "process_stat.hpp"

namespace SystemExplorer
{
    namespace Core
    {
        namespace Models
        {
            class ProcessesStat
            {
            public:
                std::map<pid_t, ProcessStat> processes_stat;
            };
        }
    }
}

#endif