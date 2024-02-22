#include <iostream>

#include "main_controller.hpp"

namespace SystemExplorer
{
    namespace Gui
    {
        namespace Controller
        {
            MainController::MainController(MainWindow *mainWindow) 
                : ControllerBase(), _mainWindow(mainWindow)
            {
              
            }

            MainController::~MainController()
            {
            }

            void MainController::CreateChildControls()
            {
                _mainBook = new wxBookCtrl(_mainWindow, wxID_ANY);
                //  Create child controllers
                //      Create process tree controller
                //      Create process details manager controller
                //      Create process details controller
            }
            void MainController::BindEvents()
            {
            }
            void MainController::BindData()
            {
            }

            void MainController::Run()
            {
                _mainWindow->Show(true);
            }

        }
    }
}