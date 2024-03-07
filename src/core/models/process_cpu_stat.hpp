#ifndef PROCESS_CPU_STAT_HPP
#define PROCESS_CPU_STAT_HPP

namespace SystemExplorer
{
    namespace Core
    {
        namespace Models
        {
            class ProcessCpuStat
            {
            public:
                float cpu_usage_per_all_cores;
                float cpu_usage_per_core;
            };
        }
    }
}

#endif