#include <istream>

#include "proc_stat_parser.hpp"

namespace SystemExplorer
{
    namespace Core
    {
        namespace Parsers
        {
            Models::ProcStat ProcStatParser::Parse(std::istream &stream)
            {
                Models::ProcStat result;
                while(true)
                {
                    Models::ProcCpuStat procCpuStat;
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