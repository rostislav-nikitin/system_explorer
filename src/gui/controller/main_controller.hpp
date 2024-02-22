#ifndef MAIN_CONTROLLER_HPP
#define MAIN_CONTROLLER_HPP

#include "../main_window.hpp"
#include "controller_base.hpp"

namespace SystemExplorer
{
    namespace Gui
    {
        namespace Controller
        {
            class MainController : public ControllerBase
            {
            private:
                MainWindow *_mainWindow;

            public:
                MainController(MainWindow *mainWindow);
                ~MainController();

                virtual void Run() override;
            };
        }
    }
}

#endif 
