#include "resources_view_controller.hpp"

namespace SystemExplorer
{
    namespace Gui
    {
        namespace ViewController
        {
            ResourcesViewController::ResourcesViewController(wxBookCtrl *book, 
                std::string title, 
                wxWindowID id, 
                bool useByDefault) : ViewControllerBase(book, title, id, useByDefault)
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
                wxVector<wxString> labels;
                labels.push_back("10");
                labels.push_back("20");
                labels.push_back("30");
                labels.push_back("40");
                labels.push_back("50");
                labels.push_back("60");
                labels.push_back("70");
                wxChartsCategoricalData::ptr chartData = wxChartsCategoricalData::make_shared(labels);
                
                // Add the first dataset
                wxVector<wxDouble> points1;
                points1.push_back(3);
                points1.push_back(-2.5);
                points1.push_back(-1.2);
                points1.push_back(3);
                points1.push_back(6);
                points1.push_back(5);
                points1.push_back(1);
                wxChartsDoubleDataset::ptr dataset1(new wxChartsDoubleDataset("My First Dataset", points1));
                chartData->AddDataset(dataset1);
                
                // Add the second dataset
                wxVector<wxDouble> points2;
                points2.push_back(1);
                points2.push_back(-1.33);
                points2.push_back(2.5);
                points2.push_back(7);
                points2.push_back(3);
                points2.push_back(-1.8);
                points2.push_back(0.4);
                wxChartsDoubleDataset::ptr dataset2(new wxChartsDoubleDataset("My Second Dataset", points2));
                chartData->AddDataset(dataset2);
                
                // Create the line chart widget from the constructed data
                _lineChartCtrl = new wxLineChartCtrl(this, wxID_ANY, chartData,
                wxCHARTSLINETYPE_STRAIGHT, wxDefaultPosition, wxDefaultSize, wxBORDER_NONE);
                
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
                static int const TIMER_INTERVAL = 5000; //ms
                _timer->Start(TIMER_INTERVAL);
            }

            void ResourcesViewController::BindEvents()
            {
                this->Bind(wxEVT_TIMER, &ResourcesViewController::_timer_OnTimer, this);
            }

            void ResourcesViewController::_timer_OnTimer(wxTimerEvent &event)
            {
                //wxMessageBox("Test", "Test");
                                // Create a top-level panel to hold all the contents of the frame
                //wxPanel* panel = new wxPanel(this, wxID_ANY);
                
                // Create the data for the line chart widget
                wxVector<wxString> labels;
                labels.push_back("10");
                labels.push_back("20");
                labels.push_back("30");
                labels.push_back("40");
                labels.push_back("50");
                labels.push_back("60");
                labels.push_back("70");
                labels.push_back("80");
                wxChartsCategoricalData::ptr chartData = wxChartsCategoricalData::make_shared(labels);
                
                // Add the first dataset
                wxVector<wxDouble> points1;
                points1.push_back(5);
                points1.push_back(-2.5);
                points1.push_back(-1.2);
                points1.push_back(3);
                points1.push_back(2);
                points1.push_back(6);
                points1.push_back(1);
                points1.push_back(10.0);
                wxChartsDoubleDataset::ptr dataset1(new wxChartsDoubleDataset("My First Dataset", points1));
                chartData->AddDataset(dataset1);
                
                // Add the second dataset
                wxVector<wxDouble> points2;
                points2.push_back(3);
                points2.push_back(-1.33);
                points2.push_back(2.5);
                points2.push_back(6);
                points2.push_back(27);
                points2.push_back(-2.1);
                points2.push_back(0.2);
                points2.push_back(6.0);
                wxChartsDoubleDataset::ptr dataset2(new wxChartsDoubleDataset("My Second Dataset", points2));
                chartData->AddDataset(dataset2);
                
                // Create the line chart widget from the constructed data
                //_lineChartCtrl->Update(chartData);
                //_panelSizer->Layout();

            }

            void ResourcesViewController::PostInitialize()
            {
                StartTimer();
            }
        }
    }
}
