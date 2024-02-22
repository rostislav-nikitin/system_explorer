#include <iostream>

#include <wx/wx.h>

#include "gui/main_window.hpp"
#include "gui/controller/main_controller.hpp"

namespace SystemExplorer
{
    class App : public wxApp
    {
    public:
    	virtual bool OnInit() override;
    };

    bool App::OnInit()
    {
        using SystemExplorer::Gui::MainWindow;
        using SystemExplorer::Gui::Controller::MainController;

        //wxFrame *frame = new wxFrame(nullptr, wxID_ANY, "wxComboCtrl and wxOwnerDrawnComboBox Sample");

        // and show it (the frames, unlike simple controls, are not shown when
        // created initially)
        //frame->Show(true);

    	if ( !wxApp::OnInit() )
	     	return false;

        MainWindow *window = new MainWindow();
        MainController controller(window);
        controller.Run();

    	return true;
    }
}

wxIMPLEMENT_APP(SystemExplorer::App);
