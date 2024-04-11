#ifndef LIBOS_STAT_PROC_STAT_MANAGER_HPP
#define LIBOS_STAT_PROC_STAT_MANAGER_HPP

#include <wait.h>

#include "model/proc_stat.hpp"
#include "model/proc_process_stat.hpp"
#include "model/proc_tree_stat.hpp"

namespace OS
{
    namespace Stat
    {
        class ProcTreeStatManager
        {
            Model::ProcTreeStat _proc_tree_stat;

            bool IsNumber(std::string str) const;

        public:
            // Get Proc[CPU] stat
            Model::ProcStat GetProcStat();

            // Get all running processes
            std::vector<pid_t> GetProcesses();

            // Get process stat
            Model::ProcProcessStat GetProcessStat(pid_t pid);

            // Get all running processes stat
            std::vector<Model::ProcProcessStat> GetProcesssesStat();

            // Get al proc tree historical stat
            Model::ProcTreeStat & GetProcTreeStat();

            // Accumulate stat on each tick
            void Tick();
        };
    }
}

#endif