#include <vector>

namespace SystemExplorer
{
    namespace Core
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
        };

        struct ProcStat
        {
            std::vector<ProcCpuStat> proc_cpu_stat;
        };
    }
}