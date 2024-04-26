#ifndef GUI_VIEW_CONTROLLER_RESOURCES_VIEW_CONTROLLER_HPP
#define GUI_VIEW_CONTROLLER_RESOURCES_VIEW_CONTROLLER_HPP

#include <string>
#include <wx/wx.h>
#include <wx/bookctrl.h>
#include <wx/charts/wxcharts.h>

#include "view_controller_base.hpp"

namespace SystemExplorer
{
    namespace Gui
    {
        namespace ViewController
        {
            class ResourcesViewController : public ViewControllerBase
            {
            public:
                ResourcesViewController(wxBookCtrl *book, 
                    std::string title, 
                    wxWindowID id = wxID_ANY, 
                    bool useByDefault = false);

                virtual void CreateChildControls() override;
            };
        }
    }
}

#endif