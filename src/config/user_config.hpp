#ifndef USER_CONFIG_HPP
#define USER_CONFIG_HPP

#include <string>
#include <set>
#include <iterator>
#include <fstream>

namespace SystemExplorer
{
    namespace Config
    {
        class UserConfig
        {
            const char * RELATIVE_CONFIG_PATH 
                = ".system_explorer";
            const char * PROCESSES_AUTOCOMPLETE_CHOICES_PATH 
                = "processes_autocomplete_choices.dat";

            static int const MAX_SIZE = 100;

            std::set<std::string> _processesAutoCompleteChoices;

            std::string GetConfigPath() const;
            std::string GetProcessesAutoCompleteChoicesPath() const;

            void LoadProcessesAutoCompleteChoices();
            void SaveProcessesAutoCompleteChoices();

        public:
            UserConfig();

            void Load();
            void Save();

            std::set<std::string> GetProcessesAutoCompleteChoices();
            void SetProcessesAutoCompleteChoices(std::set<std::string> &value);

        };
    }
}

#endif