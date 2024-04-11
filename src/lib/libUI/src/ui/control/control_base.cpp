#include "../../../include/ui/control/control_base.hpp"

namespace UI
{
    namespace Control
    {
            ControlBase::ControlBase(wxWindow *parent, wxWindowID id)
                : wxPanel(parent, id)
            {
            }

            void ControlBase::Initialize()
            {
                CreateChildControls();
                BindEvents();
            }

            void ControlBase::CreateChildControls()
            {
            }
            
            void ControlBase::BindEvents()
            {
            }
    }
}