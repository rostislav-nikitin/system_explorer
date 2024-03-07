#ifndef PROCESSES_STAT_MAANGER_HPP
#define PROCESSES_STAT_MAANGER_HPP

#include <wait.h>

#include "proc_tree_stat_manager.hpp"

#include "models/process_stat.hpp"
#include "models/processes_stat.hpp"

namespace SystemExplorer
{
    namespace Core
    {
        class ProcessesStatManager
        {
        private:
            ProcTreeStatManager _proc_tree_stat_manager;
        public:
            // Get single process stat
            Models::ProcessStat GetProcessStat(pid_t pid);

            // Get all running processes stat
            Models::ProcessesStat GetProcessesStat();

            // Capture stat on each tick
            void Tick();
        };
    }
}

#endif