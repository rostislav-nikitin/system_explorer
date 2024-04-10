#include "root_view_controller.hpp"
namespace SystemExplorer
{
    namespace Gui
    {
        namespace ViewController
        {
            RootViewController::RootViewController(Config::Config config) 
                : wxFrame(nullptr, wxID_ANY, "System Explorer", wxPoint(-1, -1), wxSize(800, 600)),
                _config(config)
            {
                using OS::Process::ProcessManager;
                using SystemExplorer::Gui::ViewController::ProcessTreeViewController;

                CreateStatusBar(1, wxSTB_SIZEGRIP|wxSTB_ELLIPSIZE_END|wxSTB_SHOW_TIPS|wxFULL_REPAINT_ON_RESIZE);


                wxBookCtrl *mainBook = new wxBookCtrl(this, wxID_ANY, wxPoint(10, 10), wxSize(800, 600));

                ProcessManager processManager;

                _processTreeViewController = new ProcessTreeViewController(
                    mainBook, processManager, "Processes", _config.GetUserConfig(), wxID_ANY, true);
                _processTreeViewController->Initialize();
            }
        }
    }
}