#include "user_config_impl.hpp"

namespace SystemExplorer
{
    namespace Config
    {
        UserConfig::UserConfig()
        {
            Load();
        }

        void UserConfig::Load()
        {
            LoadProcessesAutoCompleteChoices();
        }

        void UserConfig::Save()
        {
            SaveProcessesAutoCompleteChoices();
        }

        std::set<std::string> UserConfig::GetProcessesAutoCompleteChoices()
        {
            return _processesAutoCompleteChoices;
        }
        void UserConfig::SetProcessesAutoCompleteChoices(std::set<std::string> &value)
        {
            _processesAutoCompleteChoices = value;
        }

        std::string UserConfig::GetConfigPath() const
        {
            return FS::Directory::get_home_directory() 
                + FS::Directory::PathSeparator 
                + std::string(RELATIVE_CONFIG_PATH);
        }
        std::string UserConfig::GetProcessesAutoCompleteChoicesPath() const
        {
            return GetConfigPath() 
                + FS::Directory::PathSeparator 
                + std::string(PROCESSES_AUTOCOMPLETE_CHOICES_PATH);

        }
        void UserConfig::LoadProcessesAutoCompleteChoices()
        {
            // 01. Check user/system_explorer dir exists ~/.system_explorer/
            std::string processes_autocomplete_choices_path = GetProcessesAutoCompleteChoicesPath();

            if(FS::File::exists(processes_autocomplete_choices_path))
            {
                std::ifstream config (processes_autocomplete_choices_path);
                std::string choice;
                while(config >> choice)
                {
                    _processesAutoCompleteChoices.insert(choice);
                    //std::cout << choice << std::endl;
                }
            }
            // 02. If processes_autocomplete_choices.dat exists
            // 03. Then load data to set
            // [On autocomplete setup] Set -> wxArrayString
        }

        void UserConfig::SaveProcessesAutoCompleteChoices()
        {
            std::string autocompleteChoicesPath = GetProcessesAutoCompleteChoicesPath();
            if(!FS::File::exists(autocompleteChoicesPath))
            {
                if(!FS::Directory::exists(GetConfigPath()))
                {
                    FS::Directory::create(GetConfigPath());
                }
            }

            std::ofstream autoCompleteChoices(autocompleteChoicesPath);
            std::copy(
                _processesAutoCompleteChoices.begin(), 
                _processesAutoCompleteChoices.end(),
                std::ostream_iterator<std::string>(autoCompleteChoices, "\n"));
        }

    }
}

