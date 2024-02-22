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
            protected:
                virtual void CreateChildControls() override;
                virtual void BindEvents() override;
                virtual void BindData() override;
            public:
                virtual void Run() override;
            };
        }
    }
    
} // namespace SystemExplorer



#endif