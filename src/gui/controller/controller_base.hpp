#ifndef CONTROLLER_BASE
#define CONTROLLER_BASE

#include "icontroller.hpp"

namespace SystemExplorer
{
    namespace Gui
    {
        namespace Controller
        {
            class ControllerBase : public IController
            {
            public:
                void Run() override;
            };
        }
    }
    
} // namespace SystemExplorer



#endif