#include "config.hpp"

namespace SystemExplorer
{
    namespace Config
    {
        UserConfig &Config::GetUserConfig()
        {
            return _userConfig;
        }
    }
}