#ifndef PROCESS_CONTROLLER_HPP
#define PROCESS_CONTROLLER_HPP

#include <wx/wx.h>

namespace SystemExplorer
{
    namespace Gui
    {
        namespace Controller
        {
            class MainWindow;

            class ProcessController : public ControllerBase
            {
            private:
                wxPanel *_processesTab;

            protected:
                void CreateChildControls() override;
                void BindEvents() override;
                void BindData() override;

            public:
                ProcessController(wxPanel *processesTab);
                ~ProcessController();

            };
        }
    }
}

#endif