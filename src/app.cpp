#include <iostream>

#include <wx/wx.h>
#include <wx/bookctrl.h>

#include "core/process_manager.hpp"

#include "gui/view_controller/root_view_controller.hpp"

namespace SystemExplorer
{
    class App : public wxApp
    {
    public:
    	virtual bool OnInit() override;
    };

    bool App::OnInit()
    {
    	if ( !wxApp::OnInit() )
	     	return false;

        wxFrame *window = new SystemExplorer::Gui::ViewController::RootViewController();
        window->Show(true);

    	return true;
    }
}

wxIMPLEMENT_APP(SystemExplorer::App);
