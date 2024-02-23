#include <iostream>

#include <wx/wx.h>
#include <wx/bookctrl.h>

#include "core/process_manager.hpp"

#include "gui/main_window.hpp"
#include "gui/processes_tree_list_item_comparator.hpp"

#include "gui/controller/process_tree_controller.hpp"

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
        using SystemExplorer::Gui::Controller::ProcessTreeController;
        

        //wxFrame *frame = new wxFrame(nullptr, wxID_ANY, "wxComboCtrl and wxOwnerDrawnComboBox Sample");

        // and show it (the frames, unlike simple controls, are not shown when
        // created initially)
        //frame->Show(true);

    	if ( !wxApp::OnInit() )
	     	return false;

        // Core components
        ProcessManager processManager;

        // Gui components
        MainWindow *window = new MainWindow();
        //wxBookCtrl *mainBook = new wxBookCtrl(window, wxID_ANY);
        //ProcessTreeController processTreeController(mainBook, processManager);

        window->Show(true);

    	return true;
    }
}

wxIMPLEMENT_APP(SystemExplorer::App);
