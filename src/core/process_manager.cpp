#include "process_manager_impl.hpp"

#include <iostream>
#include <vector>

namespace SystemExplorer
{
    namespace Core
    {
        std::vector<std::string> split(std::string &str, std::string delimiter);

        ProcessTree ProcessManager::GetProcessTree(std::string filter)
        {
            ProcessTree result;

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
                    Process process(processName, pid, parentPid);
//                    std::cout << parentPid << std::endl;
                    result.processes.insert(std::make_pair(pid, process));
                }
            }

            closedir(dir);

            return result;
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
