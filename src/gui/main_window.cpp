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

            BindData();

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

            processesTreeList = new wxTreeListCtrl(processesTab, PROCESSES_TREE_ID, wxPoint(0,0), wxSize(100, 800));
            processesTreeList->SetWindowStyle(wxBORDER_NONE);
            processesTreeList->AppendColumn(_T("Name"));
            processesTreeList->AppendColumn(_T("PID"));
            //wxTreeItemId rootId = processesTreeList->AddRoot("Root");
            /*
            wxTreeListItem subItem1 = processesTreeList->AppendItem(processesTreeList->GetRootItem(), "Process #1");
            processesTreeList->SetItemText(subItem1, 1, "1");
            wxTreeListItem subItem11 = processesTreeList->AppendItem(subItem1, "Process #1");
            processesTreeList->SetItemText(subItem11, 1, "10");

            wxTreeListItem subItem2 = processesTreeList->AppendItem(processesTreeList->GetRootItem(), "Process #2");
            processesTreeList->SetItemText(subItem2, 1, "2");
            wxTreeListItem subItem22 = processesTreeList->AppendItem(subItem2, "Process #2");
            processesTreeList->SetItemText(subItem22, 1, "12");
            
            //processesTreeList->ExpandAllChildren(processesTreeList->GetRootItem());
            processesTreeList->Expand(subItem1);
            processesTreeList->Expand(subItem2);
            */

            wxBoxSizer *processesTabSizer = new wxBoxSizer(wxVERTICAL);
            processesTabSizer->Add(processesSearch, 0, wxEXPAND | wxALL, 0);
            processesTabSizer->Add(processesTreeList, 0, wxEXPAND | wxALL, 0);
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
            processesTreeList->Bind(wxEVT_KEY_DOWN, &MainWindow::processesTreeList_OnKeyDown, this, PROCESSES_TREE_ID);
            Bind(wxEVT_SEARCHCTRL_SEARCH_BTN, &MainWindow::processesSearch_Click, this, PROCESSES_SEARCH_ID);
            Bind(wxEVT_TEXT, &MainWindow::processesSearch_Click, this, PROCESSES_SEARCH_ID);
        }

        void MainWindow::processesTreeList_OnKeyDown(wxKeyEvent &event)
        {
            int keyCode = event.GetKeyCode();
            //wxMessageBox("OnDelete", std::to_string(1), wxOK | wxICON_INFORMATION, this);
            wxTreeListItem selectedItem;
            wxString selectedItemText;
            
            switch(keyCode)
            {
                case WXK_DELETE:
                    selectedItem = processesTreeList->GetSelection();
                    selectedItemText = processesTreeList->GetItemText(selectedItem);
                    wxMessageBox("OnDelete", std::to_string(keyCode) + "::" + selectedItemText, wxOK | wxICON_INFORMATION, this);
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

        void MainWindow::BindData()
        {
            using ProcessExplorer::Core::ProcessManager;
            using ProcessExplorer::Core::Models::ProcessTree;

            ProcessManager pm;
            ProcessTree processTree = pm.GetProcessTree(std::string(""));
            for(std::multimap<std::string, std::string>::const_iterator it = processTree.processes.begin(); it != processTree.processes.end(); ++it)
            {
                wxTreeListItem process = processesTreeList->AppendItem(processesTreeList->GetRootItem(), it->first);
                processesTreeList->SetItemText(process, 1, it->second);
            }
        }
    }
}
