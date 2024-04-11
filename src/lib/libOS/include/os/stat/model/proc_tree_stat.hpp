#ifndef LIBOS_STAT_MODEL_PROC_TREE_STAT_HPP
#define LIBOS_STAT_MODEL_PROC_TREE_STAT_HPP

#include <map>
#include <queue>

#include "wait.h"

#include "proc_stat.hpp"
#include "proc_process_stat.hpp"

namespace OS
{
    namespace Stat
    {
        namespace Model
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