#ifndef PROCESS_STATE_HPP
#define PROCESS_STATE_HPP

#include <map>

namespace SystemExplorer
{
    namespace Core
    {
        namespace Models
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