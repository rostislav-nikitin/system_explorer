#include "proc_tree_stat_manager_impl.hpp"

#include <iostream>

namespace SystemExplorer
{
    namespace Core
    {
        
        Models::ProcStat ProcTreeStatManager::GetProcStat()
        {
            std::ifstream proc_stat_file("/proc/stat");

            Parsers::ProcStatParser parser;
            Models::ProcStat result = parser.Parse(proc_stat_file);
            
            return result;
        }

        std::vector<pid_t> ProcTreeStatManager::GetProcesses()
        {
            std::vector<pid_t> result;

            DIR *dir;
            dir = opendir("/proc");
         
            dirent *ent;
            while((ent = readdir(dir)) != NULL)
            {
                if(IsNumber(ent->d_name))
                {
                    result.push_back(atoi(ent->d_name));
                }
            }

            closedir(dir);

            return result;

        }

        bool ProcTreeStatManager::IsNumber(std::string str) const
		{
			std::string::const_iterator it = std::find_if(str.begin(), str.end(), [](char ch) { return !std::isdigit(ch);  });
			return it == str.end();
		}

        Models::ProcProcessStat ProcTreeStatManager::GetProcessStat(pid_t pid)
        {
            std::ifstream process_stat_file(
                std::string("/proc") + std::string("/") + std::to_string(pid) 
                    + "/stat");

            Parsers::ProcProcessStatParser parser;
            Models::ProcProcessStat result = parser.Parse(process_stat_file);
            
            return result;
        }

        std::vector<Models::ProcProcessStat> ProcTreeStatManager::GetProcesssesStat()
        {
            std::vector<Models::ProcProcessStat> result;

            std::vector<pid_t> pids = ProcTreeStatManager::GetProcesses();

            std::for_each(pids.begin(), pids.end(), 
            [&result, this](pid_t pid)
            {
 //               std::cout << pid << std::endl;
                Models::ProcProcessStat stat = GetProcessStat(pid);
                result.push_back(stat);
            });

            return result;
        }


        Models::ProcTreeStat & ProcTreeStatManager::GetProcTreeStat()
        {
            return _proc_tree_stat;
        }

        void ProcTreeStatManager::Tick()
        {   
            Models::ProcStat proc_stat = GetProcStat();
            _proc_tree_stat.proc_stat.push_front(proc_stat);
            if(_proc_tree_stat.proc_stat.size() > 2)
                _proc_tree_stat.proc_stat.pop_back();

            std::vector<Models::ProcProcessStat> proc_processes_stat = GetProcesssesStat();
            std::for_each(proc_processes_stat.begin(), proc_processes_stat.end(),
                [this](Models::ProcProcessStat const &proc_process_stat)
                {
                    _proc_tree_stat
                        .proc_processes_stat[proc_process_stat.pid].push_front(proc_process_stat);

                    if(_proc_tree_stat.proc_processes_stat[proc_process_stat.pid].size() > 2)
                        _proc_tree_stat.proc_processes_stat[proc_process_stat.pid].pop_back();
                });
        }    
    }
}