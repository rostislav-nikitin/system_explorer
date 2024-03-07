#include "processes_stat_manager_impl.hpp"

#include <iterator>
#include <iostream>

namespace SystemExplorer
{
    namespace Core
    {
        // Get single process stat
        Models::ProcessStat ProcessesStatManager::GetProcessStat(pid_t pid)
        {
            Models::ProcessStat result;

            Models::ProcTreeStat &stat = _proc_tree_stat_manager.GetProcTreeStat();

            float avg_cpu_load_last = -1;
            float avg_cpu_load_before_last = -1;
            float cpu_load = -1;

            if(stat.proc_stat.size() > 1)
            {
                avg_cpu_load_last = stat.proc_stat[0].proc_cpu_stat[0].get_avg_cpu_load();
                avg_cpu_load_before_last = stat.proc_stat[1].proc_cpu_stat[0].get_avg_cpu_load();
                cpu_load = avg_cpu_load_last - avg_cpu_load_before_last;
            }

            std::deque<Models::ProcProcessStat> &q = stat.proc_processes_stat[pid];

            if(q.size() > 0)
            {
                if(cpu_load > -1)
                {
                    float process_cpu_load_last = q[0].utime + q[0].stime;
                    float process_cpu_load_before_last = q[1].utime + q[1].stime;
                    float process_cpu_load = process_cpu_load_last - process_cpu_load_before_last;

                    result.cpu_stat.cpu_usage_per_all_cores = 
                        (process_cpu_load / cpu_load);
                }
                else
                {
                    result.cpu_stat.cpu_usage_per_all_cores = -1;
                }
            }

            return result;
        }

        // Get all running processes stat
        Models::ProcessesStat ProcessesStatManager::GetProcessesStat()
        {
            Models::ProcessesStat result;

            Models::ProcTreeStat &stat = _proc_tree_stat_manager.GetProcTreeStat();

            std::for_each(stat.proc_processes_stat.begin(), stat.proc_processes_stat.end(),
            [&stat, &result, this](std::map<pid_t, std::deque<Models::ProcProcessStat>>::value_type &item)
            {
                Models::ProcessStat process_stat;

                pid_t pid = item.first;

                result.processes_stat[pid] = GetProcessStat(pid);
            });

            return result;            
        }

        // Capture stat on each tick
        void ProcessesStatManager::Tick()
        {
            _proc_tree_stat_manager.Tick();
        }
    }
}