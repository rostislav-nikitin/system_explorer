#ifndef MVC_MAIN_WINDOW_HPP
#define MVC_MAIN_WINDOW_HPP

#include <wx/wx.h>
#include <wx/bookctrl.h>

#include "../../config/config.hpp"

//#include "../../core/process_manager.hpp"
#include <os/process/process_manager.hpp>

#include "process_tree_view_controller.hpp"
#include "resources_view_controller.hpp"

namespace SystemExplorer
{
    namespace Gui
    {
        namespace ViewController
        {
            class RootViewController : public wxFrame
            {
                Config::Config _config;

                ProcessTreeViewController *_processTreeViewController;
                ResourcesViewController *_resourcesViewController;
            public:
                RootViewController(Config::Config config);
            };
        }
    }
}

#endif