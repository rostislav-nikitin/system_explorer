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
			// ExpandAll();
			StartTimer();
			SetFocus();
		}

		void MainWindow::CreateHotKeys()
		{
			using SystemExplorer::Core::SignalManager;

			//	AddHotKey(PROCESS_CONTEXT_MENU_ROOT_BASE, static_cast<int>(ProcessContextMenuId::Open), owxACCEL_CTRL, static_cast<wxKeyCode>('O'));
			AddHotKey(PROCESS_CONTEXT_MENU_SIGNAL_BASE, SignalManager::GetSignal("SIGTERM").GetId(), wxACCEL_ALT, WXK_DELETE);
			AddHotKey(PROCESS_CONTEXT_MENU_SIGNAL_BASE, SignalManager::GetSignal("SIGKILL").GetId(), wxACCEL_CTRL, WXK_DELETE);
			AddHotKey(PROCESS_CONTEXT_MENU_SIGNAL_BASE, SignalManager::GetSignal("SIGSTOP").GetId(), 
				static_cast<wxAcceleratorEntryFlags>((int)wxACCEL_SHIFT | (int)wxACCEL_CTRL), static_cast<int>('S'));
			AddHotKey(PROCESS_CONTEXT_MENU_SIGNAL_BASE, SignalManager::GetSignal("SIGCONT").GetId(), 
				static_cast<wxAcceleratorEntryFlags>((int)wxACCEL_SHIFT | (int)wxACCEL_CTRL), static_cast<int>('C'));
		}

		void MainWindow::AddHotKey(int menuBase, int itemId, wxAcceleratorEntryFlags flags, int key)
		{
			int menuItemId = menuBase + itemId;
			wxAcceleratorEntry ae(flags, key, menuItemId);
			_hotKeys.insert({menuItemId, ae});
		}

		void MainWindow::AttachMenuItem(int menuItemId, wxMenuItem *menuItem)
		{
			wxAcceleratorEntry &current = _hotKeys[menuItemId];
			current.Set(current.GetFlags(), current.GetKeyCode(), current.GetCommand(), menuItem);
		}

		void MainWindow::CreateAcceleratorTable()
		{
			std::vector<wxAcceleratorEntry> entries;

			std::for_each(_hotKeys.begin(), _hotKeys.end(),
						  [&entries](typename std::map<int, wxAcceleratorEntry>::value_type const &value)
						  {
							  entries.push_back(value.second);
							  // std::cout << value.second.ToString() << std::endl;
						  });

			entries.push_back(wxAcceleratorEntry(static_cast<wxAcceleratorEntryFlags>(
				(int)wxACCEL_CTRL | (int)wxACCEL_SHIFT), (int)'O', static_cast<int>(ProcessContextMenuId::Open)));

			//	entries.push_back(wxAcceleratorEntry(wxACCEL_CTRL, (int) 'O', static_cast<int>(ProcessContextMenuId::Open)));
			wxAcceleratorTable table(entries.size(), &entries[0]);
			// processesTreeList->SetAcceleratorTable(table);
			searchableTreeList->SetAcceleratorTable(table);

			// TODO: Implement for all hot keys (think where to attach(Window|Control)?
			// wxAcceleratorEntry es[1];
			// es[0].Set(wxACCEL_CTRL, (int) 'O', static_cast<int>(ProcessContextMenuId::Open));
			//	std::cout << es[0].ToString() << std::endl;
			// wxAcceleratorTable t(1, es);
			// processesTreeList->SetAcceleratorTable(t);
		}

		std::optional<wxAcceleratorEntry> MainWindow::GetHotKey(int menuBase, int itemId)
		{
			int menuItemId = menuBase + itemId;
			if (_hotKeys.count(menuItemId) == 0)
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

			searchableTreeList = new Control::SearchableTreeListControl(mainBook, wxID_ANY);
			mainBook->AddPage(searchableTreeList, _T("Processes(Beta)"), true);
			searchableTreeList->AppendColumn(_T("Name"), 240, wxALIGN_LEFT, wxCOL_RESIZABLE | wxCOL_SORTABLE);
			searchableTreeList->AppendColumn(_T("PID"), 100, wxALIGN_LEFT, wxCOL_RESIZABLE | wxCOL_SORTABLE);
			searchableTreeList->SetItemComparator(&_processesTreeListItemComparator);

			deamonsTab = new wxPanel(mainBook);
			mainBook->AddPage(deamonsTab, _T("Deamons"));

		}

		void MainWindow::CreateProcessesTab()
		{
			using SystemExplorer::Core::SignalManager;
			using SystemExplorer::Core::Models::Signal;
			using SystemExplorer::Core::Models::SignalType;

			processContextMenu = new wxMenu();
			processContextMenu->Append(static_cast<int>(ProcessContextMenuId::Open), wxT("&Open\tCtrl+Shift+O"), wxT("Open"));
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
												if (signal.GetSignalType().GetId() == signalType.GetId())
													AppendMenuItem(signalsMenu, signal, PROCESS_CONTEXT_MENU_SIGNAL_BASE);
											});
							  AppendSubMenu(signalTypesMenu, signalType, signalsMenu, PROCESS_CONTEXT_MENU_SIGNAL_TYPE_BASE);
						  });

			processContextMenu->Append(static_cast<int>(ProcessContextMenuId::SendSignal), wxT("Send signal"), signalTypesMenu, wxT("Send signal to the process"));
			processContextMenu->AppendSeparator();
			AppendMenuItem(processContextMenu, SignalManager::GetSignal("SIGTERM"), PROCESS_CONTEXT_MENU_SIGNAL_BASE, "Terminate");
			AppendMenuItem(processContextMenu, SignalManager::GetSignal("SIGKILL"), PROCESS_CONTEXT_MENU_SIGNAL_BASE, "Kill");
			processContextMenu->AppendSeparator();
			AppendMenuItem(processContextMenu, SignalManager::GetSignal("SIGSTOP"), PROCESS_CONTEXT_MENU_SIGNAL_BASE, "Stop");
			AppendMenuItem(processContextMenu, SignalManager::GetSignal("SIGCONT"), PROCESS_CONTEXT_MENU_SIGNAL_BASE, "Continue");

		}

		void MainWindow::CreateDeamonsTab()
		{
		}

		void MainWindow::CreateStatus()
		{
			CreateStatusBar();
			// SetStatusText(_T("Running..."));
		}

		void MainWindow::SetFocus()
		{
			searchableTreeList->SetFocus();
		}

		void MainWindow::BindEvents()
		{
			timer->Bind(wxEVT_TIMER, &MainWindow::timer_OnTick, this);
			searchableTreeList->Bind(custEVT_SEARCH, &MainWindow::searchableTreeList_Search, this); 
			searchableTreeList->Bind(custEVT_ITEM_CONTEXT_MENU, &MainWindow::searchableTreeList_OnItemContextMenu, this);
			searchableTreeList->Bind(wxEVT_MENU, &MainWindow::processContextMenu_OnMenuItem, this);
			processContextMenu->Bind(wxEVT_MENU, &MainWindow::processContextMenu_OnMenuItem, this);
			processContextMenu->Bind(wxEVT_MENU_HIGHLIGHT, &MainWindow::processesContextMenu_OnMenuHighlight, this);
			processContextMenu->Bind(wxEVT_MENU_OPEN, &MainWindow::processesContextMenu_OnMenuOpen, this);
			processContextMenu->Bind(wxEVT_MENU_CLOSE, &MainWindow::processesContextMenu_OnMenuClose, this);
		}

		void MainWindow::timer_OnTick(wxTimerEvent &event)
		{
			ReBindData();
		}

		void MainWindow::searchableTreeList_Search(wxCommandEvent &event)
		{
			BindData();
		}

		void MainWindow::searchableTreeList_OnItemContextMenu(wxCommandEvent &event)
		{
			searchableTreeList->PopupMenu(processContextMenu);
		}

		void MainWindow::processesContextMenu_OnMenuOpen(wxMenuEvent &event)
		{
		}
		void MainWindow::processesContextMenu_OnMenuClose(wxMenuEvent &event)
		{
			if (event.GetMenu() == processContextMenu)
				SetStatusText(wxT(""));
		}
		void MainWindow::processesContextMenu_OnMenuHighlight(wxMenuEvent &event)
		{
			wxMenu *menu = event.GetMenu();
			if (menu == nullptr)
			{
				SetStatusText("menu is nullptr");
				return;
			}
			int menuId = event.GetMenuId();
			if (menuId < 0)
			{
				// menuId = -menuId;
				// SetStatusText(std::to_string(menuId));
				return;
			}
			wxString help = menu->GetHelpString(menuId);
			SetStatusText(help);
		}


		pid_t MainWindow::ExtractPid(wxTreeListItem const &item) const
		{
			pid_t result = 0;

			if (item.IsOk())
			{
				wxString pidAsString = searchableTreeList->GetItemText(item, 1);
				result = std::stoi(pidAsString.ToStdString());
			}

			return result;
		}

		void MainWindow::processContextMenu_OnMenuItem(wxCommandEvent &event)
		{
			wxTreeListItems selectedItems;
			if (!searchableTreeList->GetSelections(selectedItems))
				return;

			int menuItemId = event.GetId();
			std::cout << "OnMenuItem" << menuItemId << std::endl;

			if ((menuItemId >= PROCESS_CONTEXT_MENU_ROOT_BASE) && (menuItemId < (PROCESS_CONTEXT_MENU_ROOT_BASE + PROCESS_CONTEXT_MENU_RANGE_SIZE)))
			{
				switch (static_cast<ProcessContextMenuId>(menuItemId))
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
			else if ((menuItemId >= PROCESS_CONTEXT_MENU_SIGNAL_BASE) && (menuItemId < (PROCESS_CONTEXT_MENU_SIGNAL_BASE + PROCESS_CONTEXT_MENU_RANGE_SIZE)))
			{
				int signal = menuItemId - PROCESS_CONTEXT_MENU_SIGNAL_BASE;
				SendSignalToSelectedProcesses(signal);

				// std::string text = std::to_string(signalNumber);
				// wxMessageBox(text, event.GetString(), wxOK | wxICON_INFORMATION, this);
			}
		}

		void MainWindow::SendSignalToSelectedProcesses(int signal) const
		{
			wxTreeListItems selectedItems;
			if (!searchableTreeList->GetSelections(selectedItems))
				return;

			std::for_each(selectedItems.begin(), selectedItems.end(),
						  [signal, this](wxTreeListItem const &selectedItem)
						  {
							  pid_t pid = ExtractPid(selectedItem);
							  ;
							  SystemExplorer::Core::SignalManager::SendSignal(pid, signal);
						  });
		}

		void MainWindow::BindData()
		{
			using SystemExplorer::Core::ProcessManager;
			using SystemExplorer::Core::Models::Process;
			using SystemExplorer::Core::Models::ProcessTree;

			ProcessManager pm;
			std::string searchFilter = searchableTreeList->GetSearchText();
			ProcessTree processes = pm.GetProcessTree(searchFilter);

			std::vector<Control::SearchableTreeListControl::SearchableTreeListItem> items;

			for (std::map<pid_t, Process>::const_iterator it = processes.processes.begin(); it != processes.processes.end(); ++it)
			{

				// TODO: Fill items with data
				pid_t pid = (*it).first;
				pid_t parentPid = (*it).second.GetParentPid();
				std::string text = (*it).second.GetName();
				bool matched = (*it).second.GetNameMatched();
				bool picked = (*it).second.GetPicked();
				std::vector<std::string> other({std::to_string(pid)});

				if (picked)
				{
					Control::SearchableTreeListControl::SearchableTreeListItem item(pid, text, parentPid, matched, other);
					items.push_back(item);
				}
			}

			searchableTreeList->DataBind(items);
			searchableTreeList->ExpandAll();
		}


		void MainWindow::ReBindData()
		{
			using SystemExplorer::Core::ProcessManager;
			using SystemExplorer::Core::Models::Process;
			using SystemExplorer::Core::Models::ProcessTree;

			// Get processes
			ProcessManager pm;

			std::string searchFilter = searchableTreeList->GetSearchText();
			ProcessTree processTreeToRebind = pm.GetProcessTree(searchFilter);

			std::vector<Control::SearchableTreeListControl::SearchableTreeListItem> items;
			for (std::map<pid_t, Process>::const_iterator it = processTreeToRebind.processes.begin(); it != processTreeToRebind.processes.end(); ++it)
			{

				// TODO: Fill items with data
				pid_t pid = (*it).first;
				pid_t parentPid = (*it).second.GetParentPid();
				std::string text = (*it).second.GetName();
				bool matched = (*it).second.GetNameMatched();
				bool picked = (*it).second.GetPicked();
				std::vector<std::string> other({std::to_string(pid)});

				if (picked)
				{
					Control::SearchableTreeListControl::SearchableTreeListItem item(pid, text, parentPid, matched, other);
					items.push_back(item);
				}
			}

			searchableTreeList->DataReBind(items);
//			searchableTreeList->ExpandAll();
		}

		void MainWindow::StartTimer()
		{
			if (!timer->IsRunning())
			{
				timer->Start(REFRESH_INTERVAL);
			}
		}

		void MainWindow::StopTimer()
		{
			if (timer->IsRunning())
			{
				timer->Stop();
			}
		}
	}
}
