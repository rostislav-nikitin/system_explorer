#include "root_view_controller.hpp"

#include <memory>

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
                using OS::Stat::ProcTreeStatManager;
                using OS::Stat::ProcessesStatManager;
                using SystemExplorer::Gui::ViewController::ProcessTreeViewController;

                CreateStatusBar(1, wxSTB_SIZEGRIP|wxSTB_ELLIPSIZE_END|wxSTB_SHOW_TIPS|wxFULL_REPAINT_ON_RESIZE);


                wxBookCtrl *mainBook = new wxBookCtrl(this, wxID_ANY, wxPoint(10, 10), wxSize(800, 600));

                ProcessManager processManager;
                ProcTreeStatManager procTreeStatManager;
                
                _processesStatManagerPtr = std::make_shared<ProcessesStatManager>();
                _processesStatManagerPtr->Tick();
                _processesStatManagerPtr->Tick();

                _timer = new wxTimer();
                _timer->Bind(wxEVT_TIMER, &RootViewController::timer_OnTick, this);
                _timer->Start(1000);

                _processTreeViewController = new ProcessTreeViewController(
                    mainBook, processManager, _processesStatManagerPtr, "Processes", _config.GetUserConfig(), wxID_ANY, true);
                _processTreeViewController->Initialize();

                _resourcesViewController = new ResourcesViewController(mainBook, _processesStatManagerPtr, "Resources");
                _resourcesViewController->Initialize();
            }

            void RootViewController::timer_OnTick(wxTimerEvent &event)
            {
                _processesStatManagerPtr->Tick();                
            }
        }
    }
}