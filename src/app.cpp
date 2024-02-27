#include <iostream>

#include <wx/wx.h>
#include <wx/bookctrl.h>

#include "core/process_manager.hpp"

#include "gui/main_window.hpp"
#include "gui/mvc_main_window.hpp"
#include "gui/processes_tree_list_item_comparator.hpp"

#define MVC

namespace SystemExplorer
{
    class App : public wxApp
    {
    public:
    	virtual bool OnInit() override;
    };

    bool App::OnInit()
    {
        using SystemExplorer::Core::ProcessManager;

        using SystemExplorer::Gui::MainWindow;
        //using SystemExplorer::Gui::ViewController::ProcessTreeViewController;
        

        //wxFrame *frame = new wxFrame(nullptr, wxID_ANY, "wxComboCtrl and wxOwnerDrawnComboBox Sample");

        // and show it (the frames, unlike simple controls, are not shown when
        // created initially)
        //frame->Show(true);

    	if ( !wxApp::OnInit() )
	     	return false;

        // Core components
        ProcessManager processManager;

#ifndef MVC
        // Gui components
        MainWindow *window = new MainWindow();
        window->Show(true);
#else
        wxFrame *window = new MvcMainWindow();
        window->Show(true);
#endif


    	return true;
    }
}

wxIMPLEMENT_APP(SystemExplorer::App);
