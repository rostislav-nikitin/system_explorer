#ifndef LIBOS_STAT_MODEL_PROCESSES_STAT_COMMON_HPP
#define LIBOS_STAT_MODEL_PROCESSES_STAT_COMMON_HPP

namespace OS
{
    namespace Stat
    {
        namespace Model
        {
            class ProcessesStatCommon
            {
            public:
                float cpu_load; 
                float cpu_system_load;
                double rss; 
                double vmsize; 
            };
        }
    }
}

#endif