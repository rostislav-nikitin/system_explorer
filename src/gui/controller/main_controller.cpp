#include <iostream>

#include "main_controller.hpp"

namespace SystemExplorer
{
    namespace Gui
    {
        namespace Controller
        {
            MainController::MainController(MainWindow mainWindow) : _mainWindow(mainWindow)
            {
            }

            MainController::~MainController()
            {
            }

            MainController::CreateChildControls()
            {
                std::cout << __PRETTY_FUNCTION__ << std::endl;
            }

            MainController::BindEvents()
            {
                std::cout << __PRETTY_FUNCTION__ << std::endl;
            }

            MainController::ReBindData()
            {
                std::cout << __PRETTY_FUNCTION__ << std::endl;            
            }
        }
    }
}