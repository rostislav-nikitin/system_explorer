#include "resources_view_controller.hpp"

namespace SystemExplorer
{
    namespace Gui
    {
        namespace ViewController
        {
            template<typename T>
            wxVector<T> &operator<< (wxVector<T> &vector, unsigned int positions)
            {
                //std::cout << __PRETTY_FUNCTION__ << std::endl;

                if(vector.size() == 0)
                    return vector;
                    
                for(size_t index = 0; index < vector.size() - 1; ++index)
                    vector[index] = vector[index + 1];

                vector[vector.size() - 1] = T();

                return vector;
            }

            ResourcesViewController::ResourcesViewController(wxBookCtrl *book, 
                std::shared_ptr<OS::Stat::ProcessesStatManager> processesStatManager,
                std::string title, 
                wxWindowID id, 
                bool useByDefault
                ) : ViewControllerBase(book, title, id, useByDefault), 
                    _processesStatManager(processesStatManager)
            {
            }

            void ResourcesViewController::CreateChildControls()
            {
                CreateChart();
                CreateTimer();
            }

            void ResourcesViewController::CreateChart()
            {
                // Create a top-level panel to hold all the contents of the frame
                //wxPanel* panel = new wxPanel(this, wxID_ANY);
                
                // Create the data for the line chart widget
                for(int i = 60; i > -1; --i)
                {
                    if(i % 10 == 0)
                        _labels.push_back(std::to_string(i));
                    else
                        _labels.push_back("");
                }

                wxChartsCategoricalData::ptr chartData = wxChartsCategoricalData::make_shared(_labels);

                // Add the first dataset
                for(int i = 60; i > -1; --i)
                {
                    _pointsCpuTotal.push_back(0);
                    _pointsCpuSystem.push_back(0);
                    _pointsCpuSelectedProcesses.push_back(0);
                }
                wxChartsDoubleDataset::ptr dsCpuTotal(new wxChartsDoubleDataset("CPU Total", _pointsCpuTotal));
                chartData->AddDataset(dsCpuTotal);

                wxChartsDoubleDataset::ptr dsCpuSystem(new wxChartsDoubleDataset("CPU System", _pointsCpuSystem));
                chartData->AddDataset(dsCpuSystem);

                wxChartsDoubleDataset::ptr dsCpuSelectedProcesses(new wxChartsDoubleDataset("CPU Selected Processes", _pointsCpuSelectedProcesses));
                chartData->AddDataset(dsCpuSelectedProcesses);

                
                // Create the line chart widget from the constructed data
                wxLineChartOptions chartOpts;
                chartOpts.GetCommonOptions().SetShowTooltips(false);
                wxChartsAxisOptions &axisOptions = chartOpts.GetGridOptions().GetYAxisOptions();
                axisOptions.SetExplicitStartValue(0);
                axisOptions.SetExplicitEndValue(99);

                _lineChartCtrl = new wxLineChartCtrl(this, wxID_ANY, chartData,
                wxCHARTSLINETYPE_STRAIGHT, chartOpts, wxDefaultPosition, wxDefaultSize, wxBORDER_NONE);
                
                // Create the legend widget
                wxChartsLegendData legendData(chartData->GetDatasets());
                _legendCtrl = new wxChartsLegendCtrl(this, wxID_ANY, legendData,
                wxDefaultPosition, wxSize(128, 64), wxBORDER_NONE);
                
                // Set up the sizer for the panel
                _panelSizer = new wxBoxSizer(wxVERTICAL);
                _panelSizer->Add(_lineChartCtrl, 10, wxEXPAND);
                _panelSizer->Add(_legendCtrl, 1, wxALIGN_RIGHT);
                _panelSizer->SetSizeHints(this);
                this->SetSizer(_panelSizer);

            }

            void ResourcesViewController::CreateTimer()
            {
                _timer = new wxTimer(this, wxID_ANY);
            }

            void ResourcesViewController::StartTimer()
            {
                static int const TIMER_INTERVAL = 1000; //ms
                _timer->Start(TIMER_INTERVAL);
            }

            void ResourcesViewController::BindEvents()
            {
                this->Bind(wxEVT_TIMER, &ResourcesViewController::_timer_OnTimer, this);
            }

            void ResourcesViewController::_timer_OnTimer(wxTimerEvent &event)
            {
                //_procTreeStatManager.Tick();
                
                // Create the data for the line chart widget
                
                
                wxChartsCategoricalData::ptr chartData = wxChartsCategoricalData::make_shared(_labels);

                OS::Stat::Model::ProcessesStatCommon processStatCommon = _processesStatManager->GetProcessesStatCommon();

                _pointsCpuTotal << 1;
                _pointsCpuTotal[_pointsCpuTotal.size() - 1] = (processStatCommon.cpu_load * 100.0);
                
                _pointsCpuSystem << 1;
                _pointsCpuSystem[_pointsCpuSystem.size() - 1] = (processStatCommon.cpu_system_load * 100.0);

                wxChartsDoubleDataset::ptr dsCpuTotal(new wxChartsDoubleDataset("CPU Total", _pointsCpuTotal));
                chartData->AddDataset(dsCpuTotal);

                wxChartsDoubleDataset::ptr dsCpuSystem(new wxChartsDoubleDataset("CPU System", _pointsCpuSystem));
                chartData->AddDataset(dsCpuSystem);

                wxChartsDoubleDataset::ptr dsCpuSelectedProcesses(new wxChartsDoubleDataset("CPU Selected Processes", _pointsCpuSelectedProcesses));
                chartData->AddDataset(dsCpuSelectedProcesses);
                
                // Create the line chart widget from the constructed data
                _lineChartCtrl->Update(chartData);
                //_panelSizer->Layout();

            }

            void ResourcesViewController::PostInitialize()
            {
                StartTimer();
            }
        }
    }
}
