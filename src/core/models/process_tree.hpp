#ifndef PROCESS_TREE_HPP
#define PROCESS_TREE_HPP

#include <string>
#include <map>

namespace ProcessExplorer
{
    namespace Core
    {
        namespace Models
        {
            class ProcessTree
            {
            public:
                std::multimap<std::string, std::string> processes;
            };
        }
    }
}

#endif
