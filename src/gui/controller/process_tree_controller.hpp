#ifndef PROCESS_TREE_CONTROLLER_HPP
#define PROCESS_TREE_CONTROLLER_HPP

#include <wx/wx.h>
#include <wx/accel.h>
#include <wx/bookctrl.h>
#include <wx/cshelp.h>
#include <wx/timer.h>

#include "controller_base.hpp"

#include "../../core/process_manager.hpp"

namespace SystemExplorer
{
    namespace Gui
    {
        namespace Controller
        {
            class ProcessTreeController : public ControllerBase
            {
            private:
                wxBookCtrl *_mainBook;
                SystemExplorer::Core::ProcessManager _processManager;

            protected:
                void CreateChildControls() override;
                void BindEvents() override;
                void BindData() override;

            public:
                ProcessTreeController(wxBookCtrl *mainBook, SystemExplorer::Core::ProcessManager processManager);
                ~ProcessTreeController();

            };
        }
    }
}

#endif