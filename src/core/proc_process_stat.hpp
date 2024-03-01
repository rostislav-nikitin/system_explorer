#include <string>
#include <wait.h>

namespace SystemExplorer
{
    namespace Core
    {
        struct ProcProcessStat
        {
            pid_t pid;
            std::string comm;
            char state;
            pid_t ppid;
            int pgrp;
            int session;
            int tty_nr;
            int tpgid;
            unsigned int flags;
            unsigned long int minflt;
            unsigned long int cminflt;
            unsigned long int majflt;
            unsigned long int cmajflt;
            unsigned long int utime;
            unsigned long int stime;
            long int cutime;
            long int cstime;
            long int priority;
            long int nice;
            long int num_threads;
            long int itrvalue;
            unsigned long long  starttime;
            unsigned long int vsize;
            long int rss;
            unsigned long int rsslim;
            unsigned long int startcode;
            unsigned long int endcode;
            unsigned long int startstack;
            unsigned long int kstesp;
            unsigned long int ksteip;
            unsigned long int signal;
            unsigned long int blocked;
            unsigned long int sigignore;
            unsigned long int sigcatch;
            unsigned long int wchan;
            unsigned long int snwap;
            unsigned long int cnswap;
            int exit_signal;
            int processor;
            unsigned int rt_priority;
            unsigned int policy;
            unsigned long long delayacct_blkio_ticks;
            unsigned long int guest_time;
            long int cguest_time;
            unsigned long int start_data;
            unsigned long int end_data;
            unsigned long int stack_brk;
            unsigned long int arg_start;
            unsigned long int arg_end;
            unsigned long int env_start;
            unsigned long int env_end;
            int exit_code;
        };
    }
}