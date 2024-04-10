#include "../../include/os/process/process_manager_impl.hpp"

#include <iostream>

namespace OS
{
    namespace Process
    {

        std::string get_data(dirent *ent, std::string file_name);
        std::vector<std::string> split(std::string &str, std::string delimiter);
        std::vector<Process const*> get_leafs(ProcessTree &processTree);
		std::string to_lower(std::string const &instr);
		bool name_predicate(std::string const &name, std::vector<std::string> const &filters);
		bool wild_card_name_predicate(std::string const &name, std::vector<std::string> const &filters);
		bool user_predicate(unsigned long userId, std::optional<unsigned long> filterUserId);
        void apply_filter(ProcessTree &processTree, 
			std::vector<Process const*> &leafs, 
			std::string &filter,
			std::optional<unsigned long> filterUserId);

        ProcessTree ProcessManager::GetProcessTree(std::string filter, std::optional<unsigned long> filterUserId)
        {
            ProcessTree result;
            bool filtering = filter != "";
            //std::cout << "filtering=" << filtering << std::endl;

            DIR *dir;
            dir = opendir("/proc");
         
            dirent *ent;
			std::string procPath ("/proc");
            while((ent = readdir(dir)) != NULL)
            {
                if(IsNumber(ent->d_name))
                {
					std::string fileName = procPath + "/" + std::string(ent->d_name);
					struct stat info;
					stat(fileName.c_str(), &info);  // Error check omitted
					unsigned long userId = info.st_uid;
					unsigned long groupId = info.st_gid;
					struct passwd *pw = getpwuid(userId);
					struct group  *gr = getgrgid(groupId);
					std::string userName(pw->pw_name);
					std::string groupName(gr->gr_name);

                    std::string processName = GetName(ent);
                    pid_t pid = atoi(ent->d_name);
                    pid_t parentPid = GetParentPid(ent);
					// Read /proc/{pid}/stat
                    Process process(processName, pid, parentPid, 
						userId, userName, groupId, groupName, 
						!filtering && (!filterUserId.has_value() || userId == filterUserId) );
						
                    result.processes.insert(std::make_pair(pid, process));
                }
            }

            closedir(dir);

            std::vector<Process const*> leafs = get_leafs(result);
            apply_filter(result, leafs, filter, filterUserId);

            return result;
        }

        std::vector<Process const*> get_leafs(ProcessTree &processTree)
        {
            std::vector<Process const*> result;

            for(std::map<pid_t, Process>::const_iterator it = processTree.processes.begin(); it != processTree.processes.end(); ++it)
            {
                std::map<pid_t, Process>::iterator found = 
                    std::find_if(processTree.processes.begin(), processTree.processes.end(), [it](std::pair<pid_t, Process> const &item)
                    {
                        return item.second.GetParentPid() == it->second.GetPid();
                    });

                if(found == processTree.processes.end())
                {
                    result.push_back(&(it->second));
                }
            }

            return result;
        }

        void apply_filter(ProcessTree &processTree, 
			std::vector<Process const*> &leafs, 
			std::string &filter,
			std::optional<unsigned long> filterUserId)
        {
			const std::string FILTER_DELIMITER = "|";
			std::vector<std::string> filters = split(filter, FILTER_DELIMITER);
			std::transform(filters.begin(), filters.end(), filters.begin(), to_lower);
			std::for_each(filters.begin(), filters.end(), 
				[](std::string &filter)
				{
                	if(filter.length() > 0)
                    	filter.append(1, '*');
				});
		//	std::for_each(filters.begin(), filters.end(), [](std::string const &f){ std::cout << f << std::endl; });

            std::for_each(leafs.begin(), leafs.end(), [&processTree, &filters, &filterUserId](Process const *process)
            {
                std::string name = process->GetName();
				unsigned long userId = process->GetUserId();

                if(wild_card_name_predicate(name, filters) && user_predicate(userId, filterUserId))
                {
                    pid_t pid = process->GetPid();
					processTree.processes[pid].SetNameMatched(true);
                    processTree.processes[pid].SetPicked(true);

                    pid_t parentPid = process->GetParentPid();
                    while(parentPid > 0)
                    {
                        Process *parent = &processTree.processes[parentPid];
                        processTree.processes[parentPid].SetPicked(true);
                        processTree.processes[parentPid].SetNameMatched(wild_card_name_predicate(parent->GetName(), filters));
                        parentPid = parent->GetParentPid();
                    }

//                    std::cout << pid << ":" << name << std::endl;
                }
                else
                {
                    bool isPicked = false;
                    pid_t parentPid = process->GetParentPid();
                    while(parentPid > 0)
                    {
                        Process *parent = &processTree.processes[parentPid];
						if(!isPicked && !parent->GetPicked())
						{
	                        std::string name = parent->GetName();
							unsigned long userId = parent->GetUserId();

							bool name_valid = wild_card_name_predicate(name, filters);
							bool userId_valid = user_predicate(userId, filterUserId);

	                        if(isPicked || (name_valid && userId_valid))
	                        {
	                            parent->SetPicked(true);
	                            isPicked = true;                        
	                        }
	                        if(name_valid && userId_valid)
							{
								parent->SetNameMatched(true);
								std::vector<pid_t> all_children;
								int current_idx = 0;
								all_children.push_back(parentPid);
								typedef std::map<pid_t, Process>::iterator itor;
								while(current_idx < all_children.size())
								{
									pid_t current_pid = all_children[current_idx++];
									std::for_each(processTree.processes.begin(), processTree.processes.end(), 
										[&all_children, current_pid](typename std::map<pid_t, Process>::value_type &item)
										{
											if(item.second.GetParentPid() == current_pid)
											{
												//item.second.SetNameMatched(wild_card_name_predicate(item.second.GetName(), filters));
												item.second.SetPicked(true);
												all_children.push_back(item.first);
											}
										});
								}
							}
						}
						else
						{
	                        parent->SetPicked(true);
							isPicked = true;
						}

					
                        parentPid = parent->GetParentPid();
                    }
                }
            });
/*
            for(std::map<pid_t, Process>::const_iterator it = processTree.processes.begin(); it != processTree.processes.end(); ++it)
            {
                if(it->second.GetPicked())
                    std::cout << it->second.GetPid() << ":" << it->second.GetName() << std::endl;
			}
*/
        }
      
		std::string to_lower(std::string const &instr)
		{
			std::string result;
			std::transform(instr.begin(), instr.end(), std::back_inserter(result),
			[](typename std::string::value_type c)
			{
				return std::tolower(c);
			});

			return result;
		}

		enum class StringComparison
		{
			CaseSensitive,
			CaseInsensitive
		};
		
		bool equals(std::string const &lh, std::string const &rh, StringComparison options)
		{
			switch(options)
			{
				case StringComparison::CaseSensitive:
					return lh == rh;
				case StringComparison::CaseInsensitive:
					return to_lower(lh) == to_lower(rh);
			}

			return false;
		}

		bool wild_card_name_predicate(std::string const &name, std::vector<std::string> const &filters)
		{
			std::string const lname = to_lower(name);

			std::vector<std::string>::const_iterator found_filter = 
				std::find_if(filters.begin(), filters.end(),
				[&lname](std::string const &filter)
				{
					WildCardExpr::WildCardExpr wcExpr(filter);
					//return wcExpr.find(lname) != std::string::npos;
					return wcExpr.find(lname) == 0;
				});

			return found_filter != filters.end();
		}
		
		bool name_predicate(std::string const &name, std::vector<std::string> const &filters)
		{
			std::string const lname = to_lower(name);
			std::vector<std::string>::const_iterator found_filter = std::find_if(filters.begin(), filters.end(), 
				[&lname](std::string const &filter)
				{
					std::string const lfilter = to_lower(filter);
					//std::cout << lname << "::" << lfilter << std::endl;
					
					bool found = false;
					const char PATTERN_ANY = '*';
					if((lfilter.size() > 2) && 
						(lfilter.front() == PATTERN_ANY) && 
						(lfilter.back() == PATTERN_ANY) &&
						(lname.find(lfilter.substr(1, lfilter.size() - 2), 1) != std::string::npos))
					{
						std::cout << "*----*" << std::endl;
						found = true;
					}
					else if((lfilter.size() > 1) && 
						(lfilter.back() == PATTERN_ANY) &&
						(lname.find(lfilter.substr(0, lfilter.size() - 1)) == 0))
					{
						std::cout << "-----*" << std::endl;
						found = true;
					}
					else if(lfilter.size() > 1 &&
						(lfilter.front() == PATTERN_ANY) && 
						(lname.find(lfilter.substr(1, lfilter.size() - 1), (lname.size() - (lfilter.size() - 1))) != std::string::npos))
					{
						std::cout << "*------" << std::endl;
						found = true;
					}
					else
					{
						//std::cout << "*" << std::endl;
						found = lname.find(lfilter) == 0;
					}
					
					return found;
				});

			return found_filter != filters.end();
		}

		bool user_predicate(unsigned long userId, std::optional<unsigned long> filterUserId)
		{
			if(!filterUserId.has_value())
				return true;
			
			return userId == filterUserId;
		}

		bool ProcessManager::IsNumber(std::string str)
		{
			std::string::const_iterator it = std::find_if(str.begin(), str.end(), [](char ch) { return !std::isdigit(ch);  });
			return it == str.end();
		}

		std::string ProcessManager::GetName(dirent *ent)
		{
			std::string result = get_data(ent, "comm");
			
			return result;
		}

		pid_t ProcessManager::GetParentPid(dirent *ent)
		{
			std::string data = get_data(ent, "stat");
			
			std::vector<std::string> parts = split(data, " ");
			if(parts.size() > 3)
			return atoi(parts[3].c_str());

			return 0;
		}

		std::string get_data(dirent *ent, std::string file_name)
		{
			std::string result;

			FILE *file = fopen((std::string("/proc/") + std::string(ent->d_name) + std::string("/") + file_name).c_str(), "r");
			if(file != NULL)
			{
			size_t buffer_size = 1024;
			char buffer[buffer_size + 1];
			char *buffer_ptr = reinterpret_cast<char *>(buffer);
			ssize_t size = getline(&buffer_ptr, &buffer_size, file);
			if(size > buffer_size)
			{
				size = buffer_size;
			}
			if(size > 0)
			{
				buffer[size - 1] = '\0';
			}
			else
			{
				buffer[0] = '\0';
			}

			//std::cout << buffer << std::endl;
			fclose(file);

			result = std::string(buffer);
			}
			return result;
		}

		std::vector<std::string> split(std::string &str, std::string delimiter)
		{
			std::vector<std::string> result;

			int startPos = 0;
			int endPos = 0;
			while((endPos = str.find(delimiter, startPos)) != std::string::npos)
			{
				result.push_back(str.substr(startPos, endPos - startPos));
				startPos = endPos + 1;                
				}
			if(startPos < str.size())
				result.push_back(str.substr(startPos));

			return result;
		}

	}
}
