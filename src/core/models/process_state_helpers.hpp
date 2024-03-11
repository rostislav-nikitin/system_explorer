#ifndef PROCESS_STATE_HELPERS_HPP
#define PROCESS_STATE_HELPERS_HPP

#include <string>

#include "process_state.hpp"

namespace SystemExplorer
{
    namespace Core
    {
        namespace Models
        {
            ProcessState GetProcessState(char state);
            std::string to_string(ProcessState);
        }
    }
}

#endif