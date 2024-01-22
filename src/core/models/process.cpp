#include "process_impl.hpp"

namespace SystemExplorer
{
    namespace Core
    {
        namespace Models
        {
            Process::Process() : _name(), _pid(0), _parentPid(0), _picked(false), _nameMatched(false)
            {
            }

            Process::Process(std::string name, pid_t pid, pid_t parentPid, bool picked, bool nameMatched) 
				: _name(name), _pid(pid), _parentPid(parentPid), _picked(picked), _nameMatched(nameMatched)
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

			bool Process::GetNameMatched() const
			{
				return _nameMatched;
			}
			void Process::SetNameMatched(bool value)
			{
				_nameMatched = value;
			}
        }
    }
}
