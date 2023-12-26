#include "main_window_impl.hpp"

#include <iostream>

namespace SystemExplorer
{
    namespace Gui
    {
        MainWindow::MainWindow() : wxFrame(nullptr, wxID_ANY, TITLE, wxPoint(-1, -1), wxSize(800, 600))
        {
            SetTitle(TITLE);
            CreateMainBook();
            CreateProcessesTab();
            CreateDeamonsTab();
            CreateStatus();

            BindEvents();
            //Center();
            /*Center(wxBOTH);
            CenterOnScreen();*/
        }

        void MainWindow::CreateMainBook()
        {
            int BOOK_ID = 1;
            mainBook = new wxBookCtrl(this, BOOK_ID);

            processesTab = new wxPanel(mainBook);
            mainBook->AddPage(processesTab, _T("Processes"), true);
            deamonsTab= new wxPanel(mainBook);
            mainBook->AddPage(deamonsTab, _T("Deamons"));
        }

        void MainWindow::CreateProcessesTab()
        {
            wxSearchCtrl *processesSearch = new wxSearchCtrl(processesTab, PROCESSES_SEARCH_ID, _T("Process Name"), wxPoint(0,0), wxSize(100, 32));

            processesTree = new wxTreeCtrl(processesTab, PROCESSES_TREE_ID);
            processesTree->SetWindowStyle(wxBORDER_NONE);
            wxTreeItemId rootId = processesTree->AddRoot("Root");
            processesTree->AppendItem(rootId, "Sub Node");
            processesTree->ExpandAll();

            wxBoxSizer *processesTabSizer = new wxBoxSizer(wxVERTICAL);
            processesTabSizer->Add(processesSearch, 0, wxEXPAND | wxALL, 0);
            processesTabSizer->Add(processesTree, 0, wxEXPAND | wxALL, 0);
            processesTab->SetSizer(processesTabSizer);
        }

        void MainWindow::CreateDeamonsTab()
        {
        }

        void MainWindow::CreateStatus()
        {
            CreateStatusBar();
            SetStatusText(_T("Running..."));
        }

        void MainWindow::BindEvents()
        {
            Bind(wxEVT_TREE_KEY_DOWN, &MainWindow::processesTree_OnKeyDown, this, PROCESSES_TREE_ID);
            Bind(wxEVT_SEARCHCTRL_SEARCH_BTN, &MainWindow::processesSearch_Click, this, PROCESSES_SEARCH_ID);
            Bind(wxEVT_TEXT, &MainWindow::processesSearch_Click, this, PROCESSES_SEARCH_ID);
        }

        void MainWindow::processesTree_OnKeyDown(wxTreeEvent &event)
        {
            int keyCode = event.GetKeyCode();
            switch(keyCode)
            {
                case WXK_DELETE:
                    wxMessageBox("OnDelete", std::to_string(keyCode), wxOK | wxICON_INFORMATION, this);
                    break;
                default:
                    event.Skip();
                    break;
            }
        }

        void MainWindow::processesSearch_Click(wxCommandEvent &event)
        {
            //wxMessageBox("Search", event.GetString(), wxOK | wxICON_INFORMATION, this);
            SetStatusText(event.GetString());
        }
    }
}
