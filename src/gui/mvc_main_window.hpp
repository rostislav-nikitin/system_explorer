#ifndef MVC_MAIN_WINDOW_HPP
#define MVC_MAIN_WINDOW_HPP

#include <wx/wx.h>
#include <wx/bookctrl.h>

#include "../core/process_manager.hpp"

#include "view_controller/process_tree_view_controller.hpp"

class MvcMainWindow : public wxFrame
{
    SystemExplorer::Gui::ViewController::ProcessTreeViewController *_processTreeViewController;
public:
    MvcMainWindow();
};

#endif