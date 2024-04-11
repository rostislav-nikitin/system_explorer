#include <istream>

#include "../../../include/os/stat/parser/proc_stat_parser.hpp"

namespace OS
{
    namespace Stat
    {
        namespace Parser
        {
            Model::ProcStat ProcStatParser::Parse(std::istream &stream)
            {
                Model::ProcStat result;
                while(true)
                {
                    Model::ProcCpuStat procCpuStat;
                    std::string cpu;

                    stream >> cpu;
                    if(cpu.find("cpu") != 0)
                        break;

                    stream >> procCpuStat.user;
                    stream >> procCpuStat.nice;
                    stream >> procCpuStat.system;
                    stream >> procCpuStat.idle;
                    stream >> procCpuStat.iowait;
                    stream >> procCpuStat.irq;
                    stream >> procCpuStat.softirq;
                    stream >> procCpuStat.steal;
                    stream >> procCpuStat.guest;
                    stream >> procCpuStat.guest_nice;

                    result.proc_cpu_stat.push_back(procCpuStat);
                }

                return result;                        
            }
        }
    }
}