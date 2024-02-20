#ifndef PROCESS_MANAGER_HPP
#define PROCESS_MANAGER_HPP

#include <string>

#include "models/process_tree.hpp"

namespace SystemExplorer
{
    namespace Core
    {
        using namespace Models;

        class ProcessManager
        {
            bool IsNumber(std::string str);
            std::string GetName(dirent *ent);
            pid_t GetParentPid(dirent *ent);
        public:
            ProcessTree GetProcessTree(std::string filter = "");
        };
    }
}

#endif
