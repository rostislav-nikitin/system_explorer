#ifndef LIB_OS_PROCESS_MODEL_PROCESS_TREE_HPP
#define LIB_OS_PROCESS_MODEL_PROCESS_TREE_HPP

#include <sys/types.h>
#include <string>
#include <map>

#include "process.hpp"

namespace OS
{
    namespace Process
    {
        namespace Model
        {
            class ProcessTree
            {
            public:
                std::map<pid_t, Process> processes;
            };
        }
    }
}

#endif
