#ifndef PROCESS_MANAGER_HPP
#define PROCESS_MANAGER_HPP

#include <string>
#include <dirent.h>

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

            // GetCurrentProcesses()
            // PushWithPreviousProcesses()
            //  Swap Current->Prev
            //  Fill Current with new
            
        public:
            ProcessTree GetProcessTree(std::string filter = "");
            //void Tick();
        };
    }
}

#endif
