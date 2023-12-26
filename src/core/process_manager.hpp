#include <string>

#include "models"

namespace ProcessExplorer
{
    namespace Core
    {
        using namespace Models;

        class ProcessManager
        {
        public:
            ProcessTree GetProcessTree(std::string filter);
        }
    }
}
