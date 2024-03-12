#ifndef PROCESSES_STAT_COMMON_HPP
#define PROCESSES_STAT_COMMON_HPP

namespace SystemExplorer
{
    namespace Core
    {
        namespace Models
        {
            class ProcessesStatCommon
            {
            public:
                float cpu_load; 
                double rss; 
                double vmsize; 
            };
        }
    }
}

#endif