#ifndef PROC_STAT_MANAGER_HPP
#define PROC_STAT_MANAGER_HPP

#include <wait.h>

#include "models/proc_stat.hpp"
#include "models/proc_process_stat.hpp"
#include "models/proc_tree_stat.hpp"

namespace SystemExplorer
{
    namespace Core
    {
        class ProcTreeStatManager
        {
            Models::ProcTreeStat _proc_tree_stat;

            bool IsNumber(std::string str) const;

        public:
            // Get Proc[CPU] stat
            Models::ProcStat GetProcStat();

            // Get all running processes
            std::vector<pid_t> GetProcesses();

            // Get process stat
            Models::ProcProcessStat GetProcessStat(pid_t pid);

            // Get all running processes stat
            std::vector<Models::ProcProcessStat> GetProcesssesStat();

            // Get al proc tree historical stat
            Models::ProcTreeStat & GetProcTreeStat();

            // Accumulate stat on each tick
            void Tick();
        };
    }
}

#endif