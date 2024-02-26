#ifndef MVVP_MAIN_WINDOW_HPP
#define MVVP_MAIN_WINDOW_HPP

#include <wx/wx.h>
#include <wx/bookctrl.h>

#include "../core/process_manager.hpp"

#include "view_controller/process_tree_view_controller.hpp"

class MvvpMainWindow : public wxFrame
{
    SystemExplorer::Gui::ViewController::ProcessTreeViewController *_processTreeViewController;
public:
    MvvpMainWindow();
};

#endif