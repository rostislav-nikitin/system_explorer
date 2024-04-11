#ifndef LIBOS_STAT_PROCESS_STATE_HELPERS_HPP
#define LIBOS_STAT_PROCESS_STATE_HELPERS_HPP

#include <string>

#include "model/process_state.hpp"

namespace OS
{
    namespace Stat
    {
        namespace Model
        {
            Model::ProcessState GetProcessState(char state);
            std::string to_string(Model::ProcessState);
        }
    }
}

#endif