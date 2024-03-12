#include "processes_stat_manager_impl.hpp"

#include <iterator>
#include <iostream>

namespace SystemExplorer
{
    namespace Core
    {
        Models::ProcessesStatCommon ProcessesStatManager::GetProcessesStatCommon()
        {
            Models::ProcessesStatCommon result {0.0f, 0, 0};

            Models::ProcTreeStat &stat = _proc_tree_stat_manager.GetProcTreeStat();

            // CPU stat
            if(stat.proc_stat.size() > 1)
            {
                int avg_cpu_total_load_last = stat.proc_stat[0].proc_cpu_stat[0].get_total_cpu_work_time();
                int avg_cpu_total_load_before_last = stat.proc_stat[1].proc_cpu_stat[0].get_total_cpu_work_time();

                int avg_cpu_effective_load_last = stat.proc_stat[0].proc_cpu_stat[0].get_effective_cpu_work_time();
                int avg_cpu_effective_load_before_last = stat.proc_stat[1].proc_cpu_stat[0].get_effective_cpu_work_time();

                result.cpu_load = 
                    float(avg_cpu_effective_load_last - avg_cpu_effective_load_before_last)
                    / float(avg_cpu_total_load_last - avg_cpu_total_load_before_last);
            }

            std::for_each(stat.proc_processes_stat.begin(), stat.proc_processes_stat.end(),
                [&result](std::map<pid_t, std::deque<Models::ProcProcessStat>>::value_type const &item)
                {
                    if(item.second.size() > 0)
                    {
                        result.rss += item.second[0].rss;
                        result.vmsize += item.second[0].vsize;
                    }
                });

            result.rss = result.rss * 4096 / 1024 / 1024 / 1024;
            result.vmsize = result.vmsize  / 1024 / 1024 / 1024 / 1024;

            return result;
            
        }

        // Get single process stat
        Models::ProcessStat ProcessesStatManager::GetProcessStat(pid_t pid)
        {
            Models::ProcessStat result;

            Models::ProcTreeStat &stat = _proc_tree_stat_manager.GetProcTreeStat();

            // CPU stat
            float avg_cpu_load_last = -1;
            float avg_cpu_load_before_last = -1;
            float cpu_load = -1;

            if(stat.proc_stat.size() > 1)
            {
                avg_cpu_load_last = stat.proc_stat[0].proc_cpu_stat[0].get_total_cpu_work_time();
                avg_cpu_load_before_last = stat.proc_stat[1].proc_cpu_stat[0].get_total_cpu_work_time();
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

            // Process state
            if(stat.proc_processes_stat[pid].size() > 0)
            {
                Models::ProcProcessStat &pstat = stat.proc_processes_stat[pid][0];

                result.state = Models::GetProcessState(pstat.state);
                result.threads = pstat.num_threads;

                result.priority = pstat.priority;
                result.nice = pstat.nice;

                result.mem_vsize = float(pstat.vsize) / 1024 / 1024;
                result.mem_rss = float(pstat.rss) * 4096 / 1024 / 1024;
                result.mem_pages_swapped = float(pstat.snwap) * 4096 / 1024 / 1024;
                result.processor = pstat.processor;

            }

            return result;
        }

        // Get all running processes stat
        Models::ProcessesStat ProcessesStatManager::GetProcessesStat()
        {
            Models::ProcessesStat result;

            result.processes_stat_common = GetProcessesStatCommon();

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