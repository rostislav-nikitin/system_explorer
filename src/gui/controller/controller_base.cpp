#include "controller_base.hpp"

namespace SystemExplorer
{
    namespace Gui
    {
        namespace Controller
        {
            void ControllerBase::Run()
            {
                CreateChildControls();
                BindEvents();
                BindData();
            }

            void ControllerBase::CreateChildControls()
            {
            
            }
            void ControllerBase::BindEvents()
            {

            }
            void ControllerBase::BindData()
            {
                
            }
        }
    }
}