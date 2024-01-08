#include "process_manager_impl.hpp"

#include <iostream>
#include <vector>

namespace SystemExplorer
{
    namespace Core
    {

        std::vector<std::string> split(std::string &str, std::string delimiter);
        std::vector<Process const*> get_leafs(ProcessTree &processTree);
        void apply_filter(ProcessTree &processTree, std::vector<Process const*> &leafs, std::string &filter);

        ProcessTree ProcessManager::GetProcessTree(std::string filter)
        {
            ProcessTree result;
            bool filtering = filter != "";
            std::cout << "filtering=" << filtering << std::endl;

            DIR *dir;
            dir = opendir("/proc");
         
            dirent *ent;
            while((ent = readdir(dir)) != NULL)
            {
                if(IsNumber(ent->d_name))
                {
                    //std::cout << ent->d_name << std::endl;
                    std::string processName = GetName(ent);
                    pid_t pid = atoi(ent->d_name);
                    pid_t parentPid = GetParentPid(ent);
                    Process process(processName, pid, parentPid, !filtering);
//                    std::cout << parentPid << std::endl;
                    result.processes.insert(std::make_pair(pid, process));
                }
            }

            closedir(dir);

            //if(filtering)
            //{
                std::vector<Process const*> leafs = get_leafs(result);
                apply_filter(result, leafs, filter);
            //}

            return result;
        }

        std::vector<Process const*> get_leafs(ProcessTree &processTree)
        {
            std::vector<Process const*> result;

            for(std::map<pid_t, Process>::const_iterator it = processTree.processes.begin(); it != processTree.processes.end(); ++it)
            {
                //std::string name = it->second.GetName();
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
            std::for_each(leafs.begin(), leafs.end(), [&processTree, &filter](Process const *process)
            {
                std::string name = process->GetName();
                if(name.find(filter) == 0)
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
                        std::string name = parent->GetName();
                        if(isPicked || name.find(filter) == 0)
                        {
                            parent->SetPicked(true);
                            isPicked = true;                        
                        }
                        
                        std::cout << "1:" << parentPid << std::endl;
                        parentPid = parent->GetParentPid();
                        std::cout << "2:" << parentPid << std::endl;
                    }
                }
            });

            for(std::map<pid_t, Process>::const_iterator it = processTree.processes.begin(); it != processTree.processes.end(); ++it)
            {
                if(it->second.GetPicked())
                    std::cout << it->second.GetPid() << ":" << it->second.GetName() << std::endl;
            }
        }

        bool ProcessManager::IsNumber(std::string str)
        {
            std::string::const_iterator it = std::find_if(str.begin(), str.end(), [](char ch) { return !std::isdigit(ch);  });
            return it == str.end();
        }

        std::string ProcessManager::GetName(dirent *ent)
        {
            FILE *file = fopen((std::string("/proc/") + std::string(ent->d_name) + std::string("/comm")).c_str(), "r");
            if(file != NULL)
            {
                size_t buffer_size = 255;
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
                return std::string(buffer);
            }
            return std::string();
        }

        pid_t ProcessManager::GetParentPid(dirent *ent)
        {
            FILE *file = fopen((std::string("/proc/") + std::string(ent->d_name) + std::string("/stat")).c_str(), "r");
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
                std::string data(buffer);
                std::vector<std::string> parts = split(data, " ");
                if(parts.size() > 3)
                    return atoi(parts[3].c_str());

                return 0;
            }
            return 0;
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

            return result;
        }
        
    }
}
