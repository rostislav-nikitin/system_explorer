#include "../../include/os/stat/proc_tree_stat_manager_impl.hpp"

#include <iostream>

namespace OS
{
    namespace Stat
    {
        // Get proc stat
        Model::ProcStat ProcTreeStatManager::GetProcStat()
        {
            std::ifstream proc_stat_file("/proc/stat");

            Parser::ProcStatParser parser;
            Model::ProcStat result = parser.Parse(proc_stat_file);
            
            return result;
        }

        // Get all running processes(pids) in the system
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

        // Check string is number?
        bool ProcTreeStatManager::IsNumber(std::string str) const
		{
			std::string::const_iterator it = std::find_if(str.begin(), str.end(), [](char ch) { return !std::isdigit(ch);  });
			return it == str.end();
		}

        // Get particular process stat
        Model::ProcProcessStat ProcTreeStatManager::GetProcessStat(pid_t pid)
        {
            std::ifstream process_stat_file(
                std::string("/proc") + std::string("/") + std::to_string(pid) 
                    + "/stat");

            Parser::ProcProcessStatParser parser;
            Model::ProcProcessStat result = parser.Parse(process_stat_file);
            
            return result;
        }

        // Get all processes stat
        std::vector<Model::ProcProcessStat> ProcTreeStatManager::GetProcesssesStat()
        {
            std::vector<Model::ProcProcessStat> result;

            std::vector<pid_t> pids = ProcTreeStatManager::GetProcesses();

            std::for_each(pids.begin(), pids.end(), 
            [&result, this](pid_t pid)
            {
                Model::ProcProcessStat stat = GetProcessStat(pid);
                result.push_back(stat);
            });

            return result;
        }


        Model::ProcTreeStat & ProcTreeStatManager::GetProcTreeStat()
        {
            return _proc_tree_stat;
        }

        void ProcTreeStatManager::Tick()
        {   
            // Add next proc stat to queue
            Model::ProcStat proc_stat = GetProcStat();
            _proc_tree_stat.proc_stat.push_front(proc_stat);
            if(_proc_tree_stat.proc_stat.size() > 2)
                _proc_tree_stat.proc_stat.pop_back();

            // Add next proc processes stat to queue
            std::vector<Model::ProcProcessStat> proc_processes_stat = GetProcesssesStat();
            std::for_each(proc_processes_stat.begin(), proc_processes_stat.end(),
                [this](Model::ProcProcessStat const &proc_process_stat)
                {
                    _proc_tree_stat
                        .proc_processes_stat[proc_process_stat.pid].push_front(proc_process_stat);

                    if(_proc_tree_stat.proc_processes_stat[proc_process_stat.pid].size() > 2)
                        _proc_tree_stat.proc_processes_stat[proc_process_stat.pid].pop_back();
                });

            // Delete killed/closed/etc.(not exiting anymore) processes
            std::vector<pid_t> pids_to_delete;

            std::for_each(_proc_tree_stat.proc_processes_stat.begin(), 
                _proc_tree_stat.proc_processes_stat.end(),
                [&proc_processes_stat, &pids_to_delete](std::map<pid_t, std::deque<Model::ProcProcessStat>>::value_type const &item)
                {
                    if(
                        std::find_if(proc_processes_stat.begin(), proc_processes_stat.end(), 
                            [&item](std::vector<Model::ProcProcessStat>::value_type &proc_process_stat)
                            {
                                return proc_process_stat.pid == item.first;
                            }) 
                            == proc_processes_stat.end())
                    {
                        pids_to_delete.push_back(item.first);
                    }
                            
                });
            std::for_each(pids_to_delete.begin(), pids_to_delete.end(),
                [this](pid_t pid)
                {
                    _proc_tree_stat.proc_processes_stat.erase(pid);
                });
        }    
    }
}