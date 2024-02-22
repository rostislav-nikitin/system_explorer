#ifndef PROCESS_TREE_CONTROLLER_HPP
#define PROCESS_TREE_CONTROLLER_HPP

#include <wx/wx.h>
#include <wx/accel.h>
#include <wx/bookctrl.h>
#include <wx/cshelp.h>
#include <wx/timer.h>


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

            protected:
                void CreateChildControls() override;
                void BindEvents() override;
                void BindData() override;

            public:
                ProcessTreeController(wxBookCtrl *mainBook);
                ~ProcessTreeController();

            };
        }
    }
}

#endif