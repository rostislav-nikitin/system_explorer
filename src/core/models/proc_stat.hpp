#ifndef PROC_STAT_HPP
#define PROC_STAT_HPP

#include <vector>

namespace SystemExplorer
{
    namespace Core
    {
        namespace Models
        {
            struct ProcCpuStat
            {
                unsigned long long user;
                unsigned long long nice;
                unsigned long long system;
                unsigned long long idle;
                unsigned long long iowait;
                unsigned long long irq;
                unsigned long long softirq;
                unsigned long long steal;
                unsigned long long guest;
                unsigned long long guest_nice;

                unsigned long long get_total_cpu_work_time() const
                {
                    return user + nice + system + idle + iowait + irq + softirq;
                }

                unsigned long long get_effective_cpu_work_time() const
                {
                    return user + nice + system;
                }

                float get_avg_cpu_load() const
                {
                    return float(get_effective_cpu_work_time()) / float(get_total_cpu_work_time());
                }
            };

            struct ProcStat
            {
                std::vector<ProcCpuStat> proc_cpu_stat;
            };
        }
    }
}

#endif