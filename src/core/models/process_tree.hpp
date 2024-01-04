#ifndef PROCESS_TREE_HPP
#define PROCESS_TREE_HPP

#include <sys/types.h>
#include <string>
#include <map>

#include "process.hpp"

namespace SystemExplorer
{
    namespace Core
    {
        namespace Models
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
