#include "process_manager_impl.hpp"

#include <iostream>

namespace ProcessExplorer
{
    namespace Core
    {
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
                    FILE *file = fopen((std::string("/proc/") + std::string(ent->d_name) + std::string("/comm")).c_str(), "r");
                    if(file != NULL)
                    {
                        size_t buffer_size = 255;
                        char buffer[buffer_size + 1];
                        char *buffer_ptr = reinterpret_cast<char *>(buffer);
                        ssize_t size = getline(&buffer_ptr, &buffer_size, file);
                        if(size > 0)
                        {
                            buffer[size - 1] = '\0';
                        }
                        else
                        {
                            buffer[0] = '\0';
                        }
                        //std::cout << buffer << std::endl;
                        result.processes.insert(std::make_pair(std::string(buffer), std::string(ent->d_name)));
                        fclose(file);                    
                    }
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
    }
}
