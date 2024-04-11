#ifndef LIBOS_STAT_MODEL_PROCESS_STATE_HPP
#define LIBOS_STAT_MODEL_PROCESS_STATE_HPP

#include <map>

namespace OS
{
    namespace Stat
    {
        namespace Model
        {
            enum class ProcessState
            {
                Unknown,
                Running,
                Sleeping,
                Waiting,
                Zombie,
                Stopped,
                TracingStop,
                Paging,
                Dead1,
                Dead2,
                Wakekill,
                Waking,
                Parked,
                Idle
            };
        }
    }
}

#endif