#include "process_impl.hpp"

namespace SystemExplorer
{
    namespace Core
    {
        namespace Models
        {
            Process::Process() : _name(), _pid(0), _parentPid(0), _picked(false)
            {
            }

            Process::Process(std::string name, pid_t pid, pid_t parentPid, bool picked) : _name(name), _pid(pid), _parentPid(parentPid), _picked(picked)
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

            bool Process::GetPicked() const
            {
                return _picked;
            }

            void Process::SetPicked(bool value)
            {
                _picked = value;
            }
        }
    }
}
