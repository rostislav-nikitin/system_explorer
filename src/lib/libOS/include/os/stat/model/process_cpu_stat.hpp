#ifndef LIBOS_STAT_MODEL_PROCESS_CPU_STAT_HPP
#define LIBOS_STAT_MODEL_PROCESS_CPU_STAT_HPP

namespace OS
{
    namespace Stat
    {
        namespace Model
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