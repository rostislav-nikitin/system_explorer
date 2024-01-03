#include "process_impl.hpp"

namespace SystemExplorer
{
    namespace Core
    {
        namespace Models
        {
            Process::Process(std::string name, pid_t pid, pid_t parentPid) : _name(name), _pid(pid), _parentPid(parentPid)
            {
            }

            std::string Process::GetName() const
            {
                return _name;
            }

            pid_t Process::GetPid() const
            {
                return _pid;
            }

            pid_t Process::GetParentPid() const
            {
                return _parentPid;
            }
        }
    }
}
