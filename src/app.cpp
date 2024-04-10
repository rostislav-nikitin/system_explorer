#include <iostream>

#include <wx/wx.h>
#include <wx/bookctrl.h>

#include <os/process/process_manager.hpp>

#include "resources/resources.icons.h"

#include "config/config.hpp"


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

        Config::Config config;

        wxFrame *window = new SystemExplorer::Gui::ViewController::RootViewController(config);
        window->Show(true);

        std::cout << "NEXT" << std::endl;

    	return true;
    }
}

wxIMPLEMENT_APP(SystemExplorer::App);
