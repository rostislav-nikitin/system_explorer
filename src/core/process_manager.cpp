#include "process_manager_impl.hpp"

#include <iostream>
#include <vector>

namespace SystemExplorer
{
    namespace Core
    {

        std::string get_data(dirent *ent, std::string file_name);
        std::vector<std::string> split(std::string &str, std::string delimiter);
        std::vector<Process const*> get_leafs(ProcessTree &processTree);
		bool name_predicate(std::string const &name, std::vector<std::string> const &filters);
        void apply_filter(ProcessTree &processTree, std::vector<Process const*> &leafs, std::string &filter);

        ProcessTree ProcessManager::GetProcessTree(std::string filter)
        {
            ProcessTree result;
            bool filtering = filter != "";
            //std::cout << "filtering=" << filtering << std::endl;

            DIR *dir;
            dir = opendir("/proc");
         
            dirent *ent;
            while((ent = readdir(dir)) != NULL)
            {
                if(IsNumber(ent->d_name))
                {
                    std::string processName = GetName(ent);
                    pid_t pid = atoi(ent->d_name);
                    pid_t parentPid = GetParentPid(ent);
                    Process process(processName, pid, parentPid, !filtering);
                    result.processes.insert(std::make_pair(pid, process));
                }
            }

            closedir(dir);

            std::vector<Process const*> leafs = get_leafs(result);
            apply_filter(result, leafs, filter);

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

        void apply_filter(ProcessTree &processTree, std::vector<Process const*> &leafs, std::string &filter)
        {
			const std::string FILTER_DELIMITER = "|";
			std::vector<std::string> filters = split(filter, FILTER_DELIMITER);
		//	std::for_each(filters.begin(), filters.end(), [](std::string const &f){ std::cout << f << std::endl; });

            std::for_each(leafs.begin(), leafs.end(), [&processTree, &filters](Process const *process)
            {
                std::string name = process->GetName();
			
                if(name_predicate(name, filters))
                {
                    pid_t pid = process->GetPid();
                    processTree.processes[pid].SetPicked(true);

                    pid_t parentPid = process->GetParentPid();
                    while(parentPid > 0)
                    {
                        Process *parent = &processTree.processes[parentPid];
                        processTree.processes[parentPid].SetPicked(true);
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

							bool name_valid = name_predicate(name, filters);

	                        if(isPicked || name_valid)
	                        {
	                            parent->SetPicked(true);
	                            isPicked = true;                        
	                        }
	                        if(name_valid)
							{
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

		bool name_predicate(std::string const &name, std::vector<std::string> const &filters)
		{

			std::vector<std::string>::const_iterator found_filter = std::find_if(filters.begin(), filters.end(), 
				[&name](std::string const &filter)
				{
					bool found = false;
					const std::string PATTERN_ANY = "*";
					if((filter.find(PATTERN_ANY) == (filter.size() - 1)) && (name.find(filter.substr(0, filter.size() - 1)) == 0))
					{
			//			std::cout << "-----*" << std::endl;
						found = true;
					}
					else if((filter.find(PATTERN_ANY) == 0) && (name.find(filter.substr(1, filter.size() - 1)) == (name.size() - (filter.size() - 1))))
					{
			//			std::cout << "*------" << std::endl;
						found = true;
					}
					else
					{
						std::cout << "*" << std::endl;
						found = name.find(filter) == 0;
					}
				
					return found;
				});

                return found_filter != filters.end();
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
