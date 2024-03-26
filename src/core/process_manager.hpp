#ifndef PROCESS_MANAGER_HPP
#define PROCESS_MANAGER_HPP

#include <string>
#include <optional>
#include <dirent.h>
#include <pwd.h>
#include <grp.h>

#include "models/process_tree.hpp"
#include "models/processes_stat.hpp"

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

            // GetCurrentProcesses()
            // PushWithPreviousProcesses()
            //  Swap Current->Prev
            //  Fill Current with new
            
        public:
            ProcessTree GetProcessTree(std::string filter = "", std::optional<unsigned long> filterUserId = std::nullopt);
        };
    }
}

#endif
