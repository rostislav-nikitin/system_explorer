#ifndef MAIN_CONTROLLER_HPP
#define MAIN_CONTROLLER_HPP

#include <wx/bookctrl.h>

#include "../main_window.hpp"
#include "controller_base.hpp"
#include "process_tree_controller.hpp"

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
                wxBookCtrl *_mainBook;
                
            protected:
                virtual void CreateChildControls() override;
                virtual void BindEvents() override;
                virtual void BindData() override;

            public:
                MainController(MainWindow *mainWindow);
                ~MainController();

                virtual void Run() override;
            };
        }
    }
}

#endif 
