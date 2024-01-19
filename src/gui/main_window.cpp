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

			SetFocus();
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

            processesTreeList = new wxTreeListCtrl(processesTab, PROCESSES_TREE_ID, wxPoint(0,0), wxSize(100, 800), wxTL_MULTIPLE);
            processesTreeList->SetWindowStyle(wxBORDER_NONE);
            processesTreeList->AppendColumn(_T("Name"));
            processesTreeList->AppendColumn(_T("PID"));


			processContextMenu = new wxMenu();
			processContextMenu->Append(static_cast<int>(ProcessContextMenuId::Open), wxT("&Open"), wxT("Open"));
			processContextMenu->AppendSeparator();
			processContextMenu->Append(static_cast<int>(ProcessContextMenuId::KillSigHup), wxT("Kill(SIG&HUP)"), wxT("Kill(SIGHUP)"));
			processContextMenu->Append(static_cast<int>(ProcessContextMenuId::KillSigKill), wxT("&Kill(SIGKILL)"), wxT("Kill(SIGKILL)"));

            processesTabSizer = new wxBoxSizer(wxVERTICAL);
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

		void MainWindow::SetFocus()
		{
            processesTreeList->SetFocus();
			wxTreeListItem firstItem = processesTreeList->GetFirstItem();
			if(firstItem.IsOk())
			{
				processesTreeList->Select(firstItem);	
			}
		}

        void MainWindow::BindEvents()
        {
			timer->Bind(wxEVT_TIMER, &MainWindow::timer_OnTick, this);			
            processesTreeList->Bind(wxEVT_CHAR, &MainWindow::processesTreeList_OnChar, this, PROCESSES_TREE_ID);
            processesSearch->Bind(wxEVT_SEARCHCTRL_SEARCH_BTN, &MainWindow::processesSearch_Click, this, PROCESSES_SEARCH_ID);
            processesSearch->Bind(wxEVT_TEXT, &MainWindow::processesSearch_Text, this);//, PROCESSES_SEARCH_ID);
			processesTreeList->Bind(wxEVT_TREELIST_SELECTION_CHANGED, &MainWindow::processesTreeList_OnSelectionChanged, this);
			processesTreeList->Bind(wxEVT_TREELIST_ITEM_CONTEXT_MENU, &MainWindow::precessesTreeList_OnItemContextMenu, this);
			processesTreeList->Bind(wxEVT_MENU, &MainWindow::processesTreeList_OnMenuItem, this);
        }

		void MainWindow::timer_OnTick(wxTimerEvent& event)
		{
            SetStatusText(_T("Timer tick"));
            ReBindData();
		}
		void MainWindow::processesTreeList_OnSelectionChanged(wxTreeListEvent& event)
		{
            SetStatusText(_T("Selection changed"));
			processesTreeList->GetSelections(_selectedItems);
			if(_selectedItems.size() > 0)
			{
				wxTreeListItem selectedItem = _selectedItems.back();
				if(selectedItem.IsOk())
				{
		           	wxString pidAsString = processesTreeList->GetItemText(selectedItem, 1);
		            _selectedPid = std::stoi(pidAsString.ToStdString());
		            //SetStatusText(std::to_string(_selectedPid));
					bool scrollPos = processesTreeList->GetView()->HasScrollbar(wxVERTICAL);
		            SetStatusText(std::to_string(scrollPos));
				}
			}
		}

		void MainWindow::precessesTreeList_OnItemContextMenu(wxTreeListEvent &event)
		{
			processesTreeList->PopupMenu(processContextMenu);
			SetStatusText(std::to_string(_selectedPid));
		}

        void MainWindow::processesTreeList_OnChar(wxKeyEvent &event)
        {
            int keyCode = event.GetKeyCode();
            //wxMessageBox("OnDelete", std::to_string(1), wxOK | wxICON_INFORMATION, this);
            //wxTreeListItem selectedItem;
            //wxString selectedItemText;
            //wxString pidAsString;
            //pid_t pid;
			wxTreeListItems selectedItems;
			processesTreeList->GetSelections(selectedItems);

			if(selectedItems.size() > 0)
			{
	            switch(keyCode)
	            {
	                case WXK_DELETE:
	                    //selectedItemText = processesTreeList->GetItemText(selectedItem);
	                    std::for_each(selectedItems.begin(), selectedItems.end(), [this](wxTreeListItem const &selectedItem)
							{
			                    wxString pidAsString = processesTreeList->GetItemText(selectedItem, 1);
	    		                //wxMessageBox("OnDelete", std::to_string(keyCode) + "::" + selectedItemText, wxOK | wxICON_INFORMATION, this);
	            		        pid_t pid = std::stoi(pidAsString.ToStdString());
	                    		kill(pid, SIGKILL);
							});
	                    break;
					case WXK_LEFT:
					case WXK_NUMPAD_LEFT:
                        processesTreeList->Collapse(selectedItems[0]);
						break;
					case WXK_RIGHT:
					case WXK_NUMPAD_RIGHT:
                        processesTreeList->Expand(selectedItems[0]);
						break;
	                default:

//            			wxMessageBox(wxString(std::to_string(keyCode)), "TEST", wxOK | wxICON_INFORMATION, this);
						if(event.ControlDown() && (keyCode == 1))
						{
 //           				wxMessageBox("Select all", "Select all", wxOK | wxICON_INFORMATION, this);
	           				//static int counter = 1;
		            		//SetStatusText("Ctrl-A" + std::to_string(counter++));
						}
						else if(!event.IsKeyInCategory(WXK_CATEGORY_NAVIGATION))
						{
							processesSearch->AppendText(wxString(event.GetUnicodeKey()));
						
		            		SetStatusText(event.GetUnicodeKey());
							processesSearch->SetFocus();
							processesSearch->SelectNone();
						}
					
	                    event.Skip();
	                    break;
				}
            }
        }

		void MainWindow::processesTreeList_OnMenuItem(wxCommandEvent& event)
		{
            std::string text = "Selected items count=" + std::to_string(_selectedItems.size());
            wxMessageBox(text, event.GetString(), wxOK | wxICON_INFORMATION, this);
            //wxMessageBox("MenuItem", "Menu", wxOK | wxICON_INFORMATION, this);
            SetStatusText(std::to_string(_selectedItems.size()));
		}

        void MainWindow::processesSearch_Text(wxCommandEvent &event)
        {
            //wxMessageBox("Search", event.GetString(), wxOK | wxICON_INFORMATION, this);
            SetStatusText(event.GetString());
            BindData();
        }

        void MainWindow::processesSearch_Click(wxCommandEvent &event)
		{
			SetFocus();	
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

		void MainWindow::ReBindData()
		{
            using SystemExplorer::Core::ProcessManager;
            using SystemExplorer::Core::Models::Process;
            using SystemExplorer::Core::Models::ProcessTree;

			// Get processes
            ProcessManager pm;
            std::string filter;
			filter = processesSearch->GetValue();
            ProcessTree processTree = pm.GetProcessTree(filter);

			// Cases
			// 1. Item retured
            for(std::map<pid_t, Process>::const_iterator it = processTree.processes.begin(); it != processTree.processes.end(); ++it)
            {
				bool picked = it->second.GetPicked();
				std::string name = it->second.GetName();
				pid_t pid = it->second.GetPid();
				pid_t parentPid = it->second.GetParentPid();

                if(picked)
				{
			        wxTreeListItem item = FindItemByPid(pid);
					if(!item.IsOk())
					{
						//	1.1. Picekd and not exists -- create(item)
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
						}
						else
						{
							//log error
						}
						
					}
				}
				else
				{
					//	1.2. Not picked and exists -- delete
			        wxTreeListItem item = FindItemByPid(pid);
					if(item.IsOk())
					{
						processesTreeList->DeleteItem(item);
					}
				}
			}

			// 2. Item not returned
			// 	2.1. Exists in tree -- detele
            wxTreeListItem result;
			std::vector<pid_t> toDelete;

            for(wxTreeListItem current = processesTreeList->GetFirstItem(); current.IsOk(); current = processesTreeList->GetNextItem(current))
            {
                wxString nodePid = processesTreeList->GetItemText(current, 1);
				pid_t pid = atoi(nodePid.c_str());
				auto item = std::find_if(processTree.processes.begin(), processTree.processes.end(), [pid](std::pair<const pid_t, Process> const &item){ return item.first == pid;  });
				if(item == processTree.processes.end())
				{
					toDelete.push_back(pid);					
				}
            }

			for(std::vector<pid_t>::iterator it = toDelete.begin(); it != toDelete.end(); ++it)
			{
				wxTreeListItem item = FindItemByPid(*it);
				if(item.IsOk())
				{
					processesTreeList->DeleteItem(item);
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
