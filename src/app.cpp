#include <iostream>

#include <wx/wx.h>
#include <wx/bookctrl.h>

#include "resources/resources.icons.h"

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
        wxImage::AddHandler(new wxPNGHandler()); /* plus any others needed, e.g. JPG */
        bin2c_init_RESOURCES_ICONS_H();

    	if ( !wxApp::OnInit() )
	     	return false;

        wxFrame *window = new SystemExplorer::Gui::ViewController::RootViewController();
        window->Show(true);

    	return true;
    }
}

wxIMPLEMENT_APP(SystemExplorer::App);
