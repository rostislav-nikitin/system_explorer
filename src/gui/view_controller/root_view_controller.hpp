#ifndef MVC_MAIN_WINDOW_HPP
#define MVC_MAIN_WINDOW_HPP

#include <wx/wx.h>
#include <wx/bookctrl.h>

#include "../../core/process_manager.hpp"

#include "process_tree_view_controller.hpp"

namespace SystemExplorer
{
    namespace Gui
    {
        namespace ViewController
        {
            class RootViewController : public wxFrame
            {
                ProcessTreeViewController *_processTreeViewController;
            public:
                RootViewController();
            };
        }
    }
}

#endif