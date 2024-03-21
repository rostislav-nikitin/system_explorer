#include "control_base.hpp"

namespace SystemExplorer
{
    namespace Gui
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
}