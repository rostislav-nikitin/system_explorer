#ifndef ABOUT_DIALOG_VIEW_CONTROLLER
#define ABOUT_DIALOG_VIEW_CONTROLLER

#include <wx/dialog.h>

#include "view_controller_base.hpp"

namespace SystemExplorer
{
    namespace Gui
    {
        namespace ViewController
        {
            class AboutDialogViewController : public wxDialog
            {
            protected:
                virtual void CreateChildControls();
            public:
                AboutDialogViewController(wxWindow *parent, wxWindowID id = wxID_ANY);

            };
        }
    }
}

#endif