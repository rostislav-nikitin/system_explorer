#ifndef LIBOS_STAT_PROCESSES_STAT_MAANGER_HPP
#define LIBOS_STAT_PROCESSES_STAT_MAANGER_HPP

#include <wait.h>

#include "proc_tree_stat_manager.hpp"

#include "model/process_stat.hpp"
#include "model/processes_stat.hpp"
#include "model/processes_stat_common.hpp"

namespace OS
{
    namespace Stat
    {
        class ProcessesStatManager
        {
        private:
            ProcTreeStatManager _proc_tree_stat_manager;
        public:
            // Get processes stat common like cpu load, etc.
            Model::ProcessesStatCommon GetProcessesStatCommon();
            
            // Get single process stat
            Model::ProcessStat GetProcessStat(pid_t pid);

            // Get all running processes stat
            Model::ProcessesStat GetProcessesStat();

            // Capture stat on each tick
            void Tick();
        };
    }
}

#endif