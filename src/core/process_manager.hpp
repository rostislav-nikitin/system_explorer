#ifndef PROCESS_MANAGER_HPP
#define PROCESS_MANAGER_HPP

#include <string>

#include "models/process_tree.hpp"

namespace ProcessExplorer
{
    namespace Core
    {
        using namespace Models;

        class ProcessManager
        {
        public:
            ProcessTree GetProcessTree(std::string filter);
        };
    }
}

#endif
