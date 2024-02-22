#include <iostream>

#include "main_controller.hpp"

namespace SystemExplorer
{
    namespace Gui
    {
        namespace Controller
        {
            MainController::MainController(MainWindow *mainWindow) : _mainWindow(mainWindow)
            {
            }

            MainController::~MainController()
            {
            }

            void MainController::Run()
            {
                _mainWindow->Show(true);
            }

        }
    }
}