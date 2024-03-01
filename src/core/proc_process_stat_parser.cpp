#include "proc_process_stat_parser.hpp"

namespace SystemExplorer
{
    namespace Core
    {

        ProcProcessStat ProcProcessStatParser::Parse(std::istream &stream)
        {
            ProcProcessStat result;

            stream >> result.pid;

            size_t pos = std::string::npos;
            std::string comm;
            int idx = 0;

            //TODO: Add paranthes parity checker or some protection form strange file names
            do
            {
                stream >> comm;
                if(idx != 0)
                    result.comm += " ";
                result.comm += comm;

                pos = comm.find(")");
                ++idx;
            }
            while((pos == std::string::npos) || (pos != (comm.size() - 1)));

            //stream >> result.comm;
            stream >> result.state;
            stream >> result.ppid;
            stream >> result.pgrp;
            stream >> result.session;
            stream >> result.tty_nr;
            stream >> result.tpgid;
            stream >> result.flags;
            stream >> result.minflt;
            stream >> result.cminflt;
            stream >> result.majflt;
            stream >> result.cmajflt;
            stream >> result.utime;
            stream >> result.stime;
            stream >> result.cutime;
            stream >> result.cstime;
            stream >> result.priority;
            stream >> result.nice;
            stream >> result.num_threads;
            stream >> result.itrvalue;
            stream >> result.starttime;
            stream >> result.vsize;
            stream >> result.rss;
            stream >> result.rsslim;
            stream >> result.startcode;
            stream >> result.endcode;
            stream >> result.startstack;
            stream >> result.kstesp;
            stream >> result.ksteip;
            stream >> result.signal;
            stream >> result.blocked;
            stream >> result.sigignore;
            stream >> result.sigcatch;
            stream >> result.wchan;
            stream >> result.snwap;
            stream >> result.cnswap;
            stream >> result.exit_signal;
            stream >> result.processor;
            stream >> result.rt_priority;
            stream >> result.policy;
            stream >> result.delayacct_blkio_ticks;
            stream >> result.guest_time;
            stream >> result.cguest_time;
            stream >> result.start_data;
            stream >> result.end_data;
            stream >> result.stack_brk;
            stream >> result.arg_start;
            stream >> result.arg_end;
            stream >> result.env_start;
            stream >> result.env_end;
            stream >> result.exit_code;
 
            return result;                        
        }
    }
}