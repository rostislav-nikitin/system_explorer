#include "main_window_impl.hpp"

#include <iostream>

namespace SystemExplorer
{
    namespace Gui
    {
        MainWindow::MainWindow() : wxFrame(nullptr, wxID_ANY, "System Explorer", wxPoint(-1, -1), wxSize(800, 600))
        {
            SetTitle(TITLE);

			CreateTimer();
            CreateMainBook();
            CreateProcessesTab();
            CreateDeamonsTab();
            CreateStatus();

            BindEvents();

            BindData();

			StartTimer();
        }

		void MainWindow::CreateTimer()
		{
			timer = new wxTimer();
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
            processesSearch = new wxSearchCtrl(processesTab, PROCESSES_SEARCH_ID, _T(""), wxPoint(0,0), wxSize(100, 32));
            processesSearch->SetDescriptiveText("Filter");

            processesTreeList = new wxTreeListCtrl(processesTab, PROCESSES_TREE_ID, wxPoint(0,0), wxSize(100, 800));
            processesTreeList->SetWindowStyle(wxBORDER_NONE);
            processesTreeList->AppendColumn(_T("Name"));
            processesTreeList->AppendColumn(_T("PID"));

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
			timer->Bind(wxEVT_TIMER, &MainWindow::timer_OnTick, this);			
            processesTreeList->Bind(wxEVT_KEY_DOWN, &MainWindow::processesTreeList_OnKeyDown, this, PROCESSES_TREE_ID);
//            Bind(wxEVT_SEARCHCTRL_SEARCH_BTN, &MainWindow::processesSearch_Click, this, PROCESSES_SEARCH_ID);
            processesSearch->Bind(wxEVT_TEXT, &MainWindow::processesSearch_Click, this);//, PROCESSES_SEARCH_ID);
			processesTreeList->Bind(wxEVT_TREELIST_SELECTION_CHANGED, &MainWindow::processTreeList_SelectionChanged, this);
        }

		void MainWindow::timer_OnTick(wxTimerEvent& event)
		{
            SetStatusText(_T("Timer tick"));
            BindData();
		}
		void MainWindow::processTreeList_SelectionChanged(wxTreeListEvent& event)
		{
            SetStatusText(_T("Selection changed"));
            wxTreeListItem selectedItem = processesTreeList->GetSelection();

           	wxString pidAsString = processesTreeList->GetItemText(selectedItem, 1);
            _selectedPid = std::stoi(pidAsString.ToStdString());
            //SetStatusText(std::to_string(_selectedPid));
			bool scrollPos = processesTreeList->HasScrollbar(wxVERTICAL);
            SetStatusText(std::to_string(scrollPos));
		}

        void MainWindow::processesTreeList_OnKeyDown(wxKeyEvent &event)
        {
            int keyCode = event.GetKeyCode();
            //wxMessageBox("OnDelete", std::to_string(1), wxOK | wxICON_INFORMATION, this);
            wxTreeListItem selectedItem;
            wxString selectedItemText;
            wxString pidAsString;
            pid_t pid;
            
            switch(keyCode)
            {
                case WXK_DELETE:
                    selectedItem = processesTreeList->GetSelection();
                    //selectedItemText = processesTreeList->GetItemText(selectedItem);
                    pidAsString = processesTreeList->GetItemText(selectedItem, 1);
                    //wxMessageBox("OnDelete", std::to_string(keyCode) + "::" + selectedItemText, wxOK | wxICON_INFORMATION, this);
                    pid = std::stoi(pidAsString.ToStdString());
                    kill(pid, SIGKILL);
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
            BindData();
           
        }

        void MainWindow::BindData()
        {
            using SystemExplorer::Core::ProcessManager;
            using SystemExplorer::Core::Models::Process;
            using SystemExplorer::Core::Models::ProcessTree;

            processesTreeList->DeleteAllItems();
            ProcessManager pm;
            std::string filter;
                filter = processesSearch->GetValue();

            ProcessTree processTree = pm.GetProcessTree(filter);
            for(std::map<pid_t, Process>::const_iterator it = processTree.processes.begin(); it != processTree.processes.end(); ++it)
            {
                bool picked = it->second.GetPicked();
                if(picked)
                {
                    std::string name = it->second.GetName();
                    pid_t pid = it->second.GetPid();
                    pid_t parentPid = it->second.GetParentPid();

                    wxTreeListItem parent;

                    if(parentPid == 0)
                        parent = processesTreeList->GetRootItem();
                    else
                        parent = FindItemByPid(parentPid);

                    if(parent.IsOk())
                    {
                        wxTreeListItem process = processesTreeList->AppendItem(parent, name);
                        processesTreeList->SetItemText(process, 1, std::to_string(pid));
                        processesTreeList->Expand(process);
						if(_selectedPid == pid)
							processesTreeList->Select(process);
                    }
                }
            }
        }

		void MainWindow::StartTimer()
		{
			if(!timer->IsRunning())
			{
				timer->Start(REFRESH_INTERVAL);
			}
		}

		void MainWindow::StopTimer()
		{
			if(timer->IsRunning())
			{
				timer->Stop();
			}
		}

        wxTreeListItem MainWindow::FindItemByPid(pid_t pid)
        {
            wxTreeListItem result;

            for(wxTreeListItem current = processesTreeList->GetFirstItem(); current.IsOk(); current = processesTreeList->GetNextItem(current))
            {
                wxString nodePid = processesTreeList->GetItemText(current, 1);
//                std::cout << nodePid << std::endl;
                if(atoi(nodePid.c_str()) == pid)
                {
//                    std::cout << pid << std::endl;
                    result = current;
                    break;
                }
            }

            return result;
        }

    }
}
