#ifndef MAIN_CONTROLLER_HPP
#define MAIN_CONTROLLER_HPP

#include "../main_window.hpp"
#include "icontroller.hpp"

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

            protected:
                void CreateChildControls() override;
                void BindEvents() override;
                void ReBindData() override;

            public:
                MainController(MainWindow mainWindow);
                ~MainController();

            };
        }
    }
}

#endif 
