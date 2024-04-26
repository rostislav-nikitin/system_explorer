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
                // Create a top-level panel to hold all the contents of the frame
                //wxPanel* panel = new wxPanel(this, wxID_ANY);
                
                // Create the data for the line chart widget
                wxVector<wxString> labels;
                labels.push_back("January");
                labels.push_back("February");
                labels.push_back("March");
                labels.push_back("April");
                labels.push_back("May");
                labels.push_back("June");
                labels.push_back("July");
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
                wxLineChartCtrl* lineChartCtrl = new wxLineChartCtrl(this, wxID_ANY, chartData,
                wxCHARTSLINETYPE_STRAIGHT, wxDefaultPosition, wxDefaultSize, wxBORDER_NONE);
                
                // Create the legend widget
                wxChartsLegendData legendData(chartData->GetDatasets());
                wxChartsLegendCtrl* legendCtrl = new wxChartsLegendCtrl(this, wxID_ANY, legendData,
                wxDefaultPosition, wxDefaultSize, wxBORDER_NONE);
                
                // Set up the sizer for the panel
                wxBoxSizer* panelSizer = new wxBoxSizer(wxHORIZONTAL);
                panelSizer->Add(lineChartCtrl, 1, wxEXPAND);
                panelSizer->Add(legendCtrl, 1, wxEXPAND);
                this->SetSizer(panelSizer);
            }
        }
    }
}
