#ifndef CONFIG_HPP
#define CONFIG_HPP

#include "user_config.hpp"

namespace SystemExplorer
{
    namespace Config
    {
        class Config
        {
            UserConfig _userConfig;
        public:
            UserConfig &GetUserConfig();
        };
    }
}

#endif