#ifndef PROCESS_HPP
#define PROCESS_HPP

#include <string>

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
                // From /proc/{pid}/stat
                pid_t _pid;
                std::string _name;
                char _state;
                pid_t _parentPid;

                unsigned long _userId;
                unsigned long _groupId;
                std::string _userName;
                std::string _groupName;
                
                //Additional
                bool _picked;
				bool _nameMatched;
                //TODO: Add stat queue
                //TODO: get/set stat queue size (for detailed process view with long time mon)
                //TODO: DEFAULT_QUEUE_SIZE (default has 2: prev/current to calc dx/dt)

            public:
                Process();
                Process(std::string name, pid_t pid, pid_t parentPid, 
                    unsigned long userId, std::string  userName, 
                    unsigned long groupId, std::string groupName,
                    bool picked = true, bool nameMatched = false);

                std::string GetName() const;
                pid_t GetPid() const;
                pid_t GetParentPid() const;

                unsigned long GetUserId() const;
                std::string GetUserName() const;
                unsigned long GetGroupId() const;
                std::string GetGroupName() const;


				bool GetNameMatched() const;
                bool GetPicked() const;
                void SetPicked(bool value);
				void SetNameMatched(bool value);
            };
        }
    }
}
#endif

