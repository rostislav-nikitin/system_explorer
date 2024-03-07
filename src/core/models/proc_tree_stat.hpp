#ifndef PROC_TREE_STAT_HPP
#define PROC_TREE_STAT_HPP

#include <map>
#include <queue>

#include "wait.h"

#include "proc_stat.hpp"
#include "proc_process_stat.hpp"

namespace SystemExplorer
{
    namespace Core
    {
        namespace Models
        {
            struct ProcTreeStat
            {
                std::deque<ProcStat> proc_stat;
                std::map<pid_t, std::deque<ProcProcessStat>> proc_processes_stat;
            };
        }
    }
}

#endif