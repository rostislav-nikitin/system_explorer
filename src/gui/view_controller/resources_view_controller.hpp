#ifndef GUI_VIEW_CONTROLLER_RESOURCES_VIEW_CONTROLLER_HPP
#define GUI_VIEW_CONTROLLER_RESOURCES_VIEW_CONTROLLER_HPP

#include <string>
#include <wx/wx.h>
#include <wx/bookctrl.h>
#include <wx/charts/wxcharts.h>
#include <wx/timer.h>

#include "view_controller_base.hpp"

namespace SystemExplorer
{
    namespace Gui
    {
        namespace ViewController
        {
            class ResourcesViewController : public ViewControllerBase
            {
            private:
                wxTimer *_timer;
                wxLineChartCtrl* _lineChartCtrl;
                wxChartsLegendCtrl* _legendCtrl;
                wxBoxSizer* _panelSizer;

            public:
                ResourcesViewController(wxBookCtrl *book, 
                    std::string title, 
                    wxWindowID id = wxID_ANY, 
                    bool useByDefault = false);

                virtual void CreateChildControls() override;
                virtual void BindEvents() override;
                virtual void PostInitialize() override;

                void CreateChart();
                void CreateTimer();
                void StartTimer();

                void _timer_OnTimer(wxTimerEvent &event);
            };
        }
    }
}

#endif