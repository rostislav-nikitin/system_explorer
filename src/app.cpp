#include <iostream>

#include <wx/wx.h>

#include "gui/main_window.hpp"

namespace SystemExplorer
{
    class App : public wxApp
    {
    public:
    	bool OnInit() override;
    };

    bool App::OnInit()
    {
        using SystemExplorer::Gui::MainWindow;

        MainWindow *window = new MainWindow();
        window->Show(true);

    	return true;
    }
}

wxIMPLEMENT_APP(SystemExplorer::App);
