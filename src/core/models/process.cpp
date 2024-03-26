#include "process_impl.hpp"

namespace SystemExplorer
{
    namespace Core
    {
        namespace Models
        {
            Process::Process() : _name(), _pid(0), _parentPid(0), 
                _userId(0), _userName(""), 
                _groupId(0), _groupName(""),
                _picked(false), _nameMatched(false)
            {
            }

            Process::Process(std::string name, pid_t pid, pid_t parentPid, 
                unsigned long userId, std::string userName,
                unsigned long groupId, std::string groupName,
                bool picked, bool nameMatched) 
				: _name(name), _pid(pid), _parentPid(parentPid), 
                _userId(userId), _userName(userName), 
                _groupId(groupId), _groupName(groupName),
                _picked(picked), _nameMatched(nameMatched)
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

            unsigned long Process::GetUserId() const
            {
                return _userId;
            }
            std::string Process::GetUserName() const
            {
                return _userName;
            }
            unsigned long Process::GetGroupId() const
            {
                return _groupId;
            }
            std::string Process::GetGroupName() const
            {
                return _groupName;
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
