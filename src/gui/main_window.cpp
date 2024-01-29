#include "main_window_impl.hpp"

#include <iostream>

namespace SystemExplorer
{
    namespace Gui
    {


      MainWindow::MainWindow() : wxFrame(nullptr, wxID_ANY, "System Explorer", wxPoint(-1, -1), wxSize(800, 600))
      {
	///           SetTitle(TITLE);

	CreateHotKeys();
	
	CreateTimer();
	CreateMainBook();
	CreateProcessesTab();
	CreateDeamonsTab();
	CreateStatus();

	CreateAcceleratorTable();	
	
	BindEvents();
	
	BindData();
	//ExpandAll();
	StartTimer();
	SetFocus();

      }

      void MainWindow::CreateAcceleratorTable()
      {
	//TODO: Implement for all hot keys (think where to attach(Window|Control)?
	wxAcceleratorEntry es[1];
	es[0].Set(wxACCEL_CTRL, (int) 'O', static_cast<int>(ProcessContextMenuId::Open));
	wxAcceleratorTable t(1, es);
	processesTreeList->SetAcceleratorTable(t);
      }

      void MainWindow::CreateHotKeys()
      {
	using SystemExplorer::Core::SignalManager;
	
 	AddHotKey(PROCESS_CONTEXT_MENU_SIGNAL_BASE, SignalManager::GetSignal("SIGTERM").GetId(), "Del");
	AddHotKey(PROCESS_CONTEXT_MENU_SIGNAL_BASE, SignalManager::GetSignal("SIGKILL").GetId(), "Ctrl+Del");
	AddHotKey(PROCESS_CONTEXT_MENU_SIGNAL_BASE, SignalManager::GetSignal("SIGSTOP").GetId(), "Ctrl+S");
	AddHotKey(PROCESS_CONTEXT_MENU_SIGNAL_BASE, SignalManager::GetSignal("SIGCONT").GetId(), "Ctrl+C");
			
      }
      
      void MainWindow::AddHotKey(int menuBase, int itemId, std::string hotKey)
      {
	_hotKeys.insert({menuBase + itemId, hotKey});
      }
      
      std::optional<std::string> MainWindow::GetHotKey(int menuBase, int itemId)
      {
	int menuItemId = menuBase + itemId;
	if(_hotKeys.count(menuItemId) == 0)
	  return std::nullopt;
	return std::optional(_hotKeys[menuItemId]);
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
	using SystemExplorer::Core::Models::SignalType;
	using SystemExplorer::Core::Models::Signal;
	using SystemExplorer::Core::SignalManager;

	processesSearch = new wxSearchCtrl(processesTab, PROCESSES_SEARCH_ID, _T(""), wxPoint(0,0), wxSize(100, 32));
	processesSearch->SetDescriptiveText("Filter");
	processesTreeList = new wxTreeListCtrl(processesTab, wxID_ANY, wxPoint(0,0), wxSize(100, 800), wxTL_MULTIPLE);
	processesTreeList->SetWindowStyle(wxBORDER_NONE);
	processesTreeList->AppendColumn(_T("Name"),240, wxALIGN_LEFT, wxCOL_RESIZABLE | wxCOL_SORTABLE);
	processesTreeList->AppendColumn(_T("PID"), 100, wxALIGN_LEFT, wxCOL_RESIZABLE | wxCOL_SORTABLE);

	processContextMenu = new wxMenu();
	processContextMenu->Append(static_cast<int>(ProcessContextMenuId::Open), wxT("&Open\tCtrl+O"), wxT("Open"));
	processContextMenu->AppendSeparator();

	wxMenu *signalTypesMenu = new wxMenu();

	std::vector<SignalType> signalTypes = SignalManager::GetSignalTypes();
	std::vector<Signal> signals = SignalManager::GetSignals();

	std::for_each(signalTypes.begin(), signalTypes.end(),
		      [signalTypesMenu, &signals, this](SignalType const &signalType)
		      {
			wxMenu *signalsMenu = new wxMenu();

			std::for_each(signals.begin(), signals.end(), 
				      [&signalType, signalsMenu, this](Signal &signal)
				      {
					if(signal.GetSignalType().GetId() == signalType.GetId())
					  AppendMenuItem(signalsMenu, signal, PROCESS_CONTEXT_MENU_SIGNAL_BASE);
				      });
			AppendSubMenu(signalTypesMenu, signalType, signalsMenu, PROCESS_CONTEXT_MENU_SIGNAL_TYPE_BASE);
		       
		      });

	processContextMenu->Append(static_cast<int>(ProcessContextMenuId::SendSignal), wxT("Send &signal\tCtrl+S"), signalTypesMenu, wxT("Send signal to the process"));
	processContextMenu->AppendSeparator();
	AppendMenuItem(processContextMenu,SignalManager::GetSignal("SIGTERM"),PROCESS_CONTEXT_MENU_SIGNAL_BASE, "Terminate");
	AppendMenuItem(processContextMenu,SignalManager::GetSignal("SIGKILL"),PROCESS_CONTEXT_MENU_SIGNAL_BASE, "Kill");
	processContextMenu->AppendSeparator();
	AppendMenuItem(processContextMenu,SignalManager::GetSignal("SIGSTOP"),PROCESS_CONTEXT_MENU_SIGNAL_BASE, "Stop");
	AppendMenuItem(processContextMenu,SignalManager::GetSignal("SIGCONT"),PROCESS_CONTEXT_MENU_SIGNAL_BASE, "Continue");

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
	//SetStatusText(_T("Running..."));
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
	processesTreeList->Bind(wxEVT_CHAR, &MainWindow::processesTreeList_OnChar, this);
	processesSearch->Bind(wxEVT_SEARCHCTRL_SEARCH_BTN, &MainWindow::processesSearch_Click, this, PROCESSES_SEARCH_ID);
	processesSearch->Bind(wxEVT_TEXT, &MainWindow::processesSearch_Text, this);//, PROCESSES_SEARCH_ID);
	processesTreeList->Bind(wxEVT_TREELIST_SELECTION_CHANGED, &MainWindow::processesTreeList_OnSelectionChanged, this);
	processesTreeList->Bind(wxEVT_TREELIST_ITEM_CONTEXT_MENU, &MainWindow::precessesTreeList_OnItemContextMenu, this);
	processesTreeList->Bind(wxEVT_MENU, &MainWindow::processesTreeList_OnMenuItem, this);
	processContextMenu->Bind(wxEVT_MENU_HIGHLIGHT, &MainWindow::processesContextMenu_OnMenuHighlight, this);
	processContextMenu->Bind(wxEVT_MENU_OPEN, &MainWindow::processesContextMenu_OnMenuOpen, this);
	processContextMenu->Bind(wxEVT_MENU_CLOSE, &MainWindow::processesContextMenu_OnMenuClose, this);
      }

      void MainWindow::timer_OnTick(wxTimerEvent& event)
      {
	//SetStatusText(_T("Timer tick"));
	ReBindData();
      }
      void MainWindow::processesTreeList_OnSelectionChanged(wxTreeListEvent& event)
      {
	//SetStatusText(_T("Selection changed"));
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
		//SetStatusText(std::to_string(scrollPos));
	      }
	  }
	/*wxRichToolTip tt(wxT("Tooltip"), wxT("Text\nTRTRTRR\n"));
	  tt.SetIcon(wxICON_INFORMATION);
	  tt.ShowFor(this);*/
      }

      void MainWindow::precessesTreeList_OnItemContextMenu(wxTreeListEvent &event)
      {
	processesTreeList->PopupMenu(processContextMenu);
	//SetStatusText(std::to_string(_selectedPid));

      }
      void MainWindow::processesContextMenu_OnMenuOpen(wxMenuEvent &event)
      {
	//SetStatusText(wxT(""));
      }
      void MainWindow::processesContextMenu_OnMenuClose(wxMenuEvent &event)
      {
	if(event.GetMenu() == processContextMenu)
	  SetStatusText(wxT(""));
      }
      void MainWindow::processesContextMenu_OnMenuHighlight(wxMenuEvent &event)
      {
	wxMenu *menu = event.GetMenu();
	if(menu == nullptr)
	  {
	    SetStatusText("menu is nullptr");
	    return;
	  }
	int menuId = event.GetMenuId();
	if(menuId < 0)
	  {
	    //menuId = -menuId;		
	    //SetStatusText(std::to_string(menuId));
	    return;
	  }
	wxString help = menu->GetHelpString(menuId);
	SetStatusText(help);
      }

      void MainWindow::processesTreeList_OnChar(wxKeyEvent &event)
      {
	//TODO: Implement ALT-Letter to focus on search
	int keyCode = event.GetKeyCode();
	wxTreeListItems selectedItems;
	processesTreeList->GetSelections(selectedItems);
	std::cout << keyCode << std::endl;

	switch(keyCode)
	  {
	  case WXK_DELETE:
	    if(event.ControlDown())
	      SendSignalToSelectedProcesses(SIGKILL);
	    else
	      SendSignalToSelectedProcesses(SIGTERM);
	    break;
	  case WXK_LEFT:
	  case WXK_NUMPAD_LEFT:
	    std::cout << "LEFT-> " <<  std::endl;
	    if(selectedItems.size() > 0)
	      processesTreeList->Collapse(selectedItems[0]);
	    break;
	  case WXK_RIGHT:
	  case WXK_NUMPAD_RIGHT:
	    if(selectedItems.size() > 0)
	      processesTreeList->Expand(selectedItems[0]);
	    break;
	  default:

	    //         			wxMessageBox(wxString(std::to_string(keyCode)), "TEST", wxOK | wxICON_INFORMATION, this);
	    if(event.ControlDown() && (keyCode == 1))
	      {
		//        				wxMessageBox("Select all", "Select all", wxOK | wxICON_INFORMATION, this);
		//static int counter = 1;
		//SetStatusText("Ctrl-A" + std::to_string(counter++));
	      }
	    else if(!event.IsKeyInCategory(WXK_CATEGORY_NAVIGATION) && (keyCode != WXK_TAB) && (keyCode >= WXK_SPACE))
	      {
		//            			wxMessageBox(wxString(std::to_string(keyCode)), "TEST", wxOK | wxICON_INFORMATION, this);
		processesSearch->AppendText(wxString(event.GetUnicodeKey()));
					
		//SetStatusText(event.GetUnicodeKey());
		processesSearch->SetFocus();
		processesSearch->SelectNone();
	      }
					
	    event.Skip();
	    break;
	  }
      }

      pid_t MainWindow::ExtractPid(wxTreeListItem const &item) const
      {
	pid_t result = 0;

	if(item.IsOk())
	  {
	    wxString pidAsString = processesTreeList->GetItemText(item, 1);
	    result = std::stoi(pidAsString.ToStdString());
	  }

	return result;
      }

      void MainWindow::processesTreeList_OnMenuItem(wxCommandEvent& event)
      {
	wxTreeListItems selectedItems;
	if(!processesTreeList->GetSelections(selectedItems))
	  return;

	int menuItemId = event.GetId();
	std::cout << "OnMenuItem" << menuItemId << std::endl;

			
	if((menuItemId >= PROCESS_CONTEXT_MENU_ROOT_BASE) && (menuItemId < (PROCESS_CONTEXT_MENU_ROOT_BASE + PROCESS_CONTEXT_MENU_RANGE_SIZE)))
	  {
	    switch(static_cast<ProcessContextMenuId>(menuItemId))
	      {
	      case ProcessContextMenuId::Open:
		wxMessageBox("Open Process Details", "Not Implemented Yet.", wxOK | wxICON_INFORMATION, this);
		break;
	      case ProcessContextMenuId::KillSigTerm:
		SendSignalToSelectedProcesses(SIGTERM);
		break;
	      case ProcessContextMenuId::KillSigKill:
		SendSignalToSelectedProcesses(SIGKILL);
		break;
	      }
	  }
	else if((menuItemId >= PROCESS_CONTEXT_MENU_SIGNAL_BASE) && (menuItemId < (PROCESS_CONTEXT_MENU_SIGNAL_BASE + PROCESS_CONTEXT_MENU_RANGE_SIZE)))
	  {
	    int signal = menuItemId - PROCESS_CONTEXT_MENU_SIGNAL_BASE;
	    SendSignalToSelectedProcesses(signal);
				
	    //std::string text = std::to_string(signalNumber);
	    //wxMessageBox(text, event.GetString(), wxOK | wxICON_INFORMATION, this);
	  }
      }

      void MainWindow::SendSignalToSelectedProcesses(int signal) const
      {
	wxTreeListItems selectedItems;
	if(!processesTreeList->GetSelections(selectedItems))
	  return;

	std::for_each(selectedItems.begin(), selectedItems.end(), 
		      [signal, this](wxTreeListItem const &selectedItem)
		      {
			pid_t pid = ExtractPid(selectedItem);;
			SystemExplorer::Core::SignalManager::SendSignal(pid, signal);
		      });
      }

      void MainWindow::processesSearch_Text(wxCommandEvent &event)
      {
	//wxMessageBox("Search", event.GetString(), wxOK | wxICON_INFORMATION, this);
	//SetStatusText(event.GetString());
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
		bool nameMatched = it->second.GetNameMatched();

		wxTreeListItem parent;

		if(parentPid == 0)
		  parent = processesTreeList->GetRootItem();
		else
		  parent = FindItemByPid(parentPid);

		if(parent.IsOk())
		  {
		    wxTreeListItem process = processesTreeList->AppendItem(parent, name);
		    processesTreeList->SetItemText(process, 1, std::to_string(pid));
		    //processesTreeList->SetItemData(process, new ProcessData());
		    //                        processesTreeList->Expand(process);
		    //	if(_selectedPid == pid)
		    //		processesTreeList->Select(process);
		    if(nameMatched && !filter.empty())
		      processesTreeList->Select(process);
		
		  }
	      }
	  }
	ExpandAll();
      }

      void MainWindow::ExpandAll()
      {
	/*			const pid_t InitProcessPid = 1;
				wxTreeListItem itemToExpand;
				itemToExpand = FindItemByPid(InitProcessPid);

				if(!itemToExpand.IsOk())
	*/			
	wxTreeListItem itemToExpand = processesTreeList->GetRootItem();

	if(itemToExpand.IsOk())
	  ExpandAll(itemToExpand);
      }

      void MainWindow::ExpandAll(wxTreeListItem &item)
      {
	std::vector<wxTreeListItem> nodes = GetAllSubNodes(item);
	std::for_each(nodes.begin(), nodes.end(), 
		      [this] (wxTreeListItem &item)
		      {
			processesTreeList->Expand(item);
		      });
      }

      void MainWindow::CollapseAll()
      {
	wxTreeListItem itemToExpand = processesTreeList->GetRootItem();

	if(itemToExpand.IsOk())
	  CollapseAll(itemToExpand);
      }

      void MainWindow::CollapseAll(wxTreeListItem &item)
      {
	std::vector<wxTreeListItem> nodes = GetAllSubNodes(item);
	std::for_each(nodes.begin(), nodes.end(), 
		      [this] (wxTreeListItem &item)
		      {
			processesTreeList->Collapse(item);
		      });
      }

      std::vector<wxTreeListItem> MainWindow::GetAllSubNodes(wxTreeListItem &parent)
      {
	std::vector<wxTreeListItem> result;

	if(!parent.IsOk())
	  return result;

	result.push_back(parent);
	size_t current_index = 0;
	while(current_index < result.size())
	  {
	    for(wxTreeListItem current = processesTreeList->GetFirstChild(result[current_index]); 
		current.IsOk(); 
		current = processesTreeList->GetNextSibling(current))
	      {
		if(processesTreeList->GetFirstChild(current).IsOk())
		  {
		    result.push_back(current);
		  }
	      }

	    ++current_index;
	  }

	return result;
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
