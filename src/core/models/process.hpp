#ifndef PROCESS_HPP
#define PROCESS_HPP

#include <sys/types.h>
#include <string>

namespace SystemExplorer
{
    namespace Core
    {
        namespace Models
        {
            class Process
            {
                std::string _name;
                pid_t _pid;
                pid_t _parentPid;
                bool _picked;

            public:
                Process();
                Process(std::string name, pid_t pid, pid_t parentPid, bool picked = true);

                std::string GetName() const;
                pid_t GetPid() const;
                pid_t GetParentPid() const;

                bool GetPicked() const;
                void SetPicked(bool value);
            };
        }
    }
}
#endif

