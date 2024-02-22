#include "controller_base.hpp"

namespace SystemExplorer
{
    namespace Gui
    {
        namespace Controller
        {
            ControllerBase::ControllerBase()
            {
                CreateChildControls();
                BindEvents();
                BindData();
            }

            void ControllerBase::Run()
            {
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