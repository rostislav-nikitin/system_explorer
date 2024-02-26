#include "process_tree_view_controller.hpp"

namespace SystemExplorer
{
    namespace Gui
    {
        namespace ViewController
        {
            ProcessTreeViewController::ProcessTreeViewController(wxBookCtrl *book, 
                SystemExplorer::Core::ProcessManager processManager,
                std::string title,
                wxWindowID id,
                bool useByDefault) 
                : ViewControllerBase(book, title, id, useByDefault), _processManager(processManager)
            {
                //TODO: Move code here
                std::cout << __PRETTY_FUNCTION__ << std::endl;

            }
            ProcessTreeViewController::~ProcessTreeViewController()
            {
            
            }

            void ProcessTreeViewController::CreateChildControls()
            {
            	CreateHotKeys();

			    CreateTimer();
				CreateProcessesTreeList();
				CreateAcceleratorTable();
            }

            void ProcessTreeViewController::PostInitialize()
            {
       			StartTimer();
    			SetFocus();
            }


       		void ProcessTreeViewController::CreateHotKeys()
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

            void ProcessTreeViewController::AddHotKey(int menuBase, int itemId, wxAcceleratorEntryFlags flags, int key)
            {
                int menuItemId = menuBase + itemId;
                wxAcceleratorEntry ae(flags, key, menuItemId);
                _hotKeys.insert({menuItemId, ae});
            }

            void ProcessTreeViewController::AttachMenuItem(int menuItemId, wxMenuItem *menuItem)
            {
                wxAcceleratorEntry &current = _hotKeys[menuItemId];
                current.Set(current.GetFlags(), current.GetKeyCode(), current.GetCommand(), menuItem);
            }

            void ProcessTreeViewController::CreateAcceleratorTable()
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
                _searchableTreeList->SetAcceleratorTable(table);

                // TODO: Implement for all hot keys (think where to attach(Window|Control)?
                // wxAcceleratorEntry es[1];
                // es[0].Set(wxACCEL_CTRL, (int) 'O', static_cast<int>(ProcessContextMenuId::Open));
                //	std::cout << es[0].ToString() << std::endl;
                // wxAcceleratorTable t(1, es);
                // processesTreeList->SetAcceleratorTable(t);
            }

            std::optional<wxAcceleratorEntry> ProcessTreeViewController::GetHotKey(int menuBase, int itemId)
            {
                int menuItemId = menuBase + itemId;
                if (_hotKeys.count(menuItemId) == 0)
                    return std::nullopt;
                return std::optional(_hotKeys[menuItemId]);
            }

            void ProcessTreeViewController::CreateTimer()
            {
                _timer = new wxTimer();
            }

            void ProcessTreeViewController::CreateProcessesTreeList()
            {
                using SystemExplorer::Core::SignalManager;
                using SystemExplorer::Core::Models::Signal;
                using SystemExplorer::Core::Models::SignalType;

                std::cout << __PRETTY_FUNCTION__ << std::endl;
                
       			_searchableTreeList = new Control::SearchableTreeListControl(this, wxID_ANY);
                std::cout << _book << std::endl;
                //wxPanel *panel = new wxPanel(_mainBook, )
    			
	    		_searchableTreeList->AppendColumn(_T("Name"), 240, wxALIGN_LEFT, wxCOL_RESIZABLE | wxCOL_SORTABLE);
		    	_searchableTreeList->AppendColumn(_T("PID"), 100, wxALIGN_LEFT, wxCOL_RESIZABLE | wxCOL_SORTABLE);
			    _searchableTreeList->SetItemComparator(&_processesTreeListItemComparator);

                _bsSizer = new wxBoxSizer(wxVERTICAL);
                _bsSizer->Add(_searchableTreeList, 1, wxEXPAND | wxALL, 0);
                this->SetSizer(_bsSizer);

                _processContextMenu = new wxMenu();
                _processContextMenu->Append(static_cast<int>(ProcessContextMenuId::Open), wxT("&Open\tCtrl+Shift+O"), wxT("Open"));
                _processContextMenu->AppendSeparator();

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

                _processContextMenu->Append(static_cast<int>(ProcessContextMenuId::SendSignal), wxT("Send signal"), signalTypesMenu, wxT("Send signal to the process"));
                _processContextMenu->AppendSeparator();
                AppendMenuItem(_processContextMenu, SignalManager::GetSignal("SIGTERM"), PROCESS_CONTEXT_MENU_SIGNAL_BASE, "Terminate");
                AppendMenuItem(_processContextMenu, SignalManager::GetSignal("SIGKILL"), PROCESS_CONTEXT_MENU_SIGNAL_BASE, "Kill");
                _processContextMenu->AppendSeparator();
                AppendMenuItem(_processContextMenu, SignalManager::GetSignal("SIGSTOP"), PROCESS_CONTEXT_MENU_SIGNAL_BASE, "Stop");
                AppendMenuItem(_processContextMenu, SignalManager::GetSignal("SIGCONT"), PROCESS_CONTEXT_MENU_SIGNAL_BASE, "Continue");

            }

            void ProcessTreeViewController::SetFocus()
            {
                _searchableTreeList->SetFocus();
            }

            void ProcessTreeViewController::BindEvents()
            {
                _searchableTreeList->Bind(custEVT_SEARCH, &ProcessTreeViewController::searchableTreeList_Search, this); 
                _searchableTreeList->Bind(custEVT_ITEM_CONTEXT_MENU, &ProcessTreeViewController::searchableTreeList_OnItemContextMenu, this);
                _searchableTreeList->Bind(wxEVT_MENU, &ProcessTreeViewController::processesContextMenu_OnMenuItem, this);
                _processContextMenu->Bind(wxEVT_MENU_HIGHLIGHT, &ProcessTreeViewController::processesContextMenu_OnMenuHighlight, this);
                _processContextMenu->Bind(wxEVT_MENU_OPEN, &ProcessTreeViewController::processesContextMenu_OnMenuOpen, this);
                _processContextMenu->Bind(wxEVT_MENU_CLOSE, &ProcessTreeViewController::processesContextMenu_OnMenuClose, this);
                _timer->Bind(wxEVT_TIMER, &ProcessTreeViewController::timer_OnTick, this);
            }

            void ProcessTreeViewController::timer_OnTick(wxTimerEvent &event)
            {
                std::cout << __PRETTY_FUNCTION__ << std::endl;
                std::cout << typeid(this->_searchableTreeList).name() << std::endl;
                std::cout << __PRETTY_FUNCTION__ << std::endl;
                ReBindData();
            }

            void ProcessTreeViewController::searchableTreeList_Search(wxCommandEvent &event)
            {
                std::cout << __PRETTY_FUNCTION__ << std::endl;
                BindData();
            }

            void ProcessTreeViewController::searchableTreeList_OnItemContextMenu(wxCommandEvent &event)
            {
                _searchableTreeList->PopupMenu(_processContextMenu);
            }

            void ProcessTreeViewController::processesContextMenu_OnMenuOpen(wxMenuEvent &event)
            {
            }
            void ProcessTreeViewController::processesContextMenu_OnMenuClose(wxMenuEvent &event)
            {
                //TODO: Implement status text manipulations
                //if (event.GetMenu() == processContextMenu)
                    //SetStatusText(wxT(""));
            }

            void ProcessTreeViewController::processesContextMenu_OnMenuHighlight(wxMenuEvent &event)
            {
                wxMenu *menu = event.GetMenu();
                if (menu == nullptr)
                {
                    return;
                }
                int menuId = event.GetMenuId();
                if (menuId < 0)
                {
                    return;
                }
                wxString help = menu->GetHelpString(menuId);
                // TODO: Implement SetStatusText
                // SetStatusText(help);
            }


            pid_t ProcessTreeViewController::ExtractPid(wxTreeListItem const &item) const
            {
                pid_t result = 0;

                if (item.IsOk())
                {
                    wxString pidAsString = _searchableTreeList->GetItemText(item, 1);
                    result = std::stoi(pidAsString.ToStdString());
                }

                return result;
            }

            void ProcessTreeViewController::processesContextMenu_OnMenuItem(wxCommandEvent &event)
            {
                wxTreeListItems selectedItems;
                if (!_searchableTreeList->GetSelections(selectedItems))
                    return;

                int menuItemId = event.GetId();
                std::cout << "OnMenuItem" << menuItemId << std::endl;

                if ((menuItemId >= PROCESS_CONTEXT_MENU_ROOT_BASE) && (menuItemId < (PROCESS_CONTEXT_MENU_ROOT_BASE + PROCESS_CONTEXT_MENU_RANGE_SIZE)))
                {
                    switch (static_cast<ProcessContextMenuId>(menuItemId))
                    {
                    case ProcessContextMenuId::Open:
                        wxMessageBox("Open Process Details", "Not Implemented Yet.", wxOK | wxICON_INFORMATION, this->_book->GetParent());
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

            void ProcessTreeViewController::SendSignalToSelectedProcesses(int signal) const
            {
                wxTreeListItems selectedItems;
                if (!_searchableTreeList->GetSelections(selectedItems))
                    return;

                std::for_each(selectedItems.begin(), selectedItems.end(),
                            [signal, this](wxTreeListItem const &selectedItem)
                            {
                                pid_t pid = ExtractPid(selectedItem);
                                ;
                                SystemExplorer::Core::SignalManager::SendSignal(pid, signal);
                            });
            }


            void ProcessTreeViewController::BindData()
            {

                using SystemExplorer::Core::ProcessManager;
                using SystemExplorer::Core::Models::Process;
                using SystemExplorer::Core::Models::ProcessTree;

                ProcessManager pm;
                std::string searchFilter = _searchableTreeList->GetSearchText();
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

                _searchableTreeList->DataBind(items);
                _searchableTreeList->ExpandAll();
            }


            void ProcessTreeViewController::ReBindData()
            {
                using SystemExplorer::Core::ProcessManager;
                using SystemExplorer::Core::Models::Process;
                using SystemExplorer::Core::Models::ProcessTree;

                // Get processes
                ProcessManager pm;
                std::cout << "RBD::01" << std::endl;

                std::string searchFilter = _searchableTreeList->GetSearchText();
                std::cout << "RBD::02" << std::endl;
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

                std::cout << "RBD::03" << std::endl;
                _searchableTreeList->DataReBind(items);
    //			_searchableTreeList->ExpandAll();
            }

            void ProcessTreeViewController::StartTimer()
            {
                if (!_timer->IsRunning())
                {
                    _timer->Start(REFRESH_INTERVAL);
                }
            }

            void ProcessTreeViewController::StopTimer()
            {
                if (_timer->IsRunning())
                {
                    _timer->Stop();
                }
            }

        }
    }
}
