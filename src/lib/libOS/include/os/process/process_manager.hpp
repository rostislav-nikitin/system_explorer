#ifndef LIB_OS_PROCESS_PROCESS_MANAGER_HPP
#define LIB_OS_PROCESS_PROCESS_MANAGER_HPP

#include <string>
#include <optional>
#include <dirent.h>
#include <pwd.h>
#include <grp.h>

#include "model/process_tree.hpp"

namespace OS
{
    namespace Process
    {
        using namespace Model;

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
