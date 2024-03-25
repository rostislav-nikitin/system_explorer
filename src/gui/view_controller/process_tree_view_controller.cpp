#include "process_tree_view_controller.hpp"
#include <sstream>

namespace SystemExplorer
{
    namespace Gui
    {
        namespace ViewController
        {
            ProcessTreeViewController::ProcessTreeViewController(wxBookCtrl *book, 
                SystemExplorer::Core::ProcessManager processManager,
                std::string title,
                Config::UserConfig &userConfig,
                wxWindowID id,
                bool useByDefault) 
                : ViewControllerBase(book, title, id, useByDefault), 
                    _processManager(processManager),
                    _userConfig(userConfig),
                    _sbStatIndex(-1),
                    _viewState(ViewState::List)
            {
            }

            ProcessTreeViewController::~ProcessTreeViewController()
            {
            }

            bool ProcessTreeViewController::UpdateAutoCompleteChoices()
            {
                bool result = false;

                std::set<std::string> _autoCompleteChoices = _userConfig.GetProcessesAutoCompleteChoices();

                if(_autoCompleteChoices.find(
                    _processesListControl->GetSearchText()) == _autoCompleteChoices.end())
                {
                    _autoCompleteChoices.insert(_processesListControl->GetSearchText());
                    result = true;
                }

                _userConfig.SetProcessesAutoCompleteChoices(_autoCompleteChoices);

                return result;
            }

            void ProcessTreeViewController::SetAutoCompleteChoices()
            {
                wxArrayString autoCompleteChoicesArray;
                std::set<std::string> autoCompleteChoices = _userConfig.GetProcessesAutoCompleteChoices();

                std::copy(
                    autoCompleteChoices.begin(), 
                    autoCompleteChoices.end(), 
                    std::back_inserter(autoCompleteChoicesArray));

                if(autoCompleteChoicesArray.size() > 0)
                    _processesListControl->AutoComplete(autoCompleteChoicesArray);
            }

            void ProcessTreeViewController::CreateChildControls()
            {
                _processesStatManager.Tick();

                CreateStatusBarField();
            	CreateHotKeys();

			    CreateTimer();
                CreateImageList();
				CreateProcessesTreeList();
                CreateMainSizer();
                CreateContextMenu();
				CreateAcceleratorTable();
            }

            void ProcessTreeViewController::PostInitialize()
            {
       			StartTimer();
    			SetFocus();
                _processesStatManager.Tick();
            }

            void ProcessTreeViewController::CreateStatusBarField()
            {
                _statusBar = GetStatusBar();
                if(_statusBar == nullptr)
                {
                    wxFrame *topFrame = GetTopFrame();
                    topFrame->CreateStatusBar();
                }

                int oldFieldsCount = _statusBar->GetFieldsCount();
                int fieldsCount = oldFieldsCount + 2;

                _sbStatIndex = oldFieldsCount;

                int widths_field[fieldsCount];
                //std::fill_n(&widths_field[0], fieldsCount, 160);
                widths_field[0] = -1;
                widths_field[1] = 144;
                widths_field[2] = 176;
                _statusBar->SetFieldsCount(fieldsCount, &widths_field[0]);

                wxRect rect_cpu;
                _statusBar->GetFieldRect(1, rect_cpu);
                /*
                wxPoint point_cpu(rect_cpu.GetPosition());
                point_cpu.x += 30;
                point_cpu.y += 2;
                wxSize size_cpu(rect_cpu.GetSize());
                size_cpu.SetWidth(size_cpu.GetWidth() - 64);
	            
	            _gProgressBarCpu = new wxGauge(_statusBar, wxID_ANY, 100, point_cpu, size_cpu);
                _gProgressBarCpu->SetBackgroundColour(wxTransparentColour);
                _gProgressBarCpu->SetForegroundColour(wxColour(186, 255, 255, wxALPHA_TRANSPARENT));
                _gProgressBarCpu->SetValue(50);
                */
                wxRect rect_ram;
                _statusBar->GetFieldRect(2, rect_ram);
                /*
                wxPoint point_rss(rect_ram.GetPosition());
                point_rss.x += 14;
                point_rss.y += 1;
                wxSize size_rss(rect_ram.GetSize());
                size_rss.SetWidth(rect_ram.GetWidth() - 14);

                _gProgressBarRss = new wxGauge(_statusBar, wxID_ANY, 100, point_rss, size_rss, wxGA_SMOOTH);
                _gProgressBarRss->SetBackgroundColour(wxTransparentColour);
                _gProgressBarRss->SetForegroundColour(wxColour(225, 223, 255, 255));
                _gProgressBarRss->SetValue(25);
                */

                _cpuStat = new Control::IconControl(_statusBar, wxID_ANY, *bin2c_cpu_png, "CPU: N/A", rect_cpu.GetPosition(), wxSize(144, 24));
                _ramStat = new Control::IconControl(_statusBar, wxID_ANY, *bin2c_ram_png, "RAM: N/A", rect_ram.GetPosition(), wxSize(176, 24));
            }

       		void ProcessTreeViewController::CreateHotKeys()
            {
                using SystemExplorer::Core::SignalManager;

                //	AddHotKey(PROCESS_CONTEXT_MENU_ROOT_BASE, static_cast<int>(ProcessContextMenuId::Open), owxACCEL_CTRL, static_cast<wxKeyCode>('O'));
                AddHotKey(PROCESS_CONTEXT_MENU_SIGNAL_BASE, SignalManager::GetSignal("SIGTERM").GetId(), wxACCEL_ALT, WXK_DELETE);
                AddHotKey(PROCESS_CONTEXT_MENU_SIGNAL_BASE, SignalManager::GetSignal("SIGKILL").GetId(), wxACCEL_CTRL, WXK_DELETE);
                AddHotKey(PROCESS_CONTEXT_MENU_SIGNAL_BASE, SignalManager::GetSignal("SIGSTOP").GetId(), 
                    static_cast<wxAcceleratorEntryFlags>((int)wxACCEL_CTRL), static_cast<int>('P'));
                AddHotKey(PROCESS_CONTEXT_MENU_SIGNAL_BASE, SignalManager::GetSignal("SIGCONT").GetId(), 
                    static_cast<wxAcceleratorEntryFlags>((int)wxACCEL_CTRL), static_cast<int>('U'));
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
                            });

                entries.push_back(wxAcceleratorEntry(static_cast<wxAcceleratorEntryFlags>(
                    (int)wxACCEL_CTRL), (int)'O', static_cast<int>(ProcessContextMenuId::Open)));
                entries.push_back(wxAcceleratorEntry(static_cast<wxAcceleratorEntryFlags>(
                    (int)wxACCEL_CTRL), (int)'E', static_cast<int>(ProcessContextMenuId::ExpandAll)));
                entries.push_back(wxAcceleratorEntry(static_cast<wxAcceleratorEntryFlags>(
                    (int)wxACCEL_CTRL), (int)'T', static_cast<int>(ProcessContextMenuId::ToggleView)));
                entries.push_back(wxAcceleratorEntry(static_cast<wxAcceleratorEntryFlags>(
                    (int)wxACCEL_CTRL), (int)'L', static_cast<int>(ProcessContextMenuId::CollapseAll)));
                entries.push_back(wxAcceleratorEntry(static_cast<wxAcceleratorEntryFlags>(
                    (int)wxACCEL_CTRL | (int)wxACCEL_SHIFT), (int)'A', static_cast<int>(ProcessContextMenuId::About)));
                entries.push_back(wxAcceleratorEntry(static_cast<wxAcceleratorEntryFlags>(
                    (int)wxACCEL_ALT), WXK_F4, static_cast<int>(ProcessContextMenuId::Close)));

                //	entries.push_back(wxAcceleratorEntry(wxACCEL_CTRL, (int) 'O', static_cast<int>(ProcessContextMenuId::Open)));
                wxAcceleratorTable table(entries.size(), &entries[0]);
                // processesTreeList->SetAcceleratorTable(table);
                _processesListControl->SetAcceleratorTable(table);

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

            void ProcessTreeViewController::CreateImageList()
            {
                _imageList = new wxImageList(16, 16, false, 32);
                // N/A
                _imageList->Add(*bin2c_realtime_png);
                _imageList->Add(*bin2c_not_nice_png);
                _imageList->Add(*bin2c_normal_png);
                _imageList->Add(*bin2c_nice_png);
                // Sleeping
                _imageList->Add(*bin2c_realtime_sleeping_png);
                _imageList->Add(*bin2c_not_nice_sleeping_png);
                _imageList->Add(*bin2c_normal_sleeping_png);
                _imageList->Add(*bin2c_nice_sleeping_png);
                // Waiting
                _imageList->Add(*bin2c_realtime_waiting_png);
                _imageList->Add(*bin2c_not_nice_waiting_png);
                _imageList->Add(*bin2c_normal_waiting_png);
                _imageList->Add(*bin2c_nice_waiting_png);
                // Idle
                _imageList->Add(*bin2c_realtime_idle_png);
                _imageList->Add(*bin2c_not_nice_idle_png);
                _imageList->Add(*bin2c_normal_idle_png);
                _imageList->Add(*bin2c_nice_idle_png);
                // Running
                _imageList->Add(*bin2c_realtime_running_png);
                _imageList->Add(*bin2c_not_nice_running_png);
                _imageList->Add(*bin2c_normal_running_png);
                _imageList->Add(*bin2c_nice_running_png);
                // Stopped
                _imageList->Add(*bin2c_realtime_stopped_png);
                _imageList->Add(*bin2c_not_nice_stopped_png);
                _imageList->Add(*bin2c_normal_stopped_png);
                _imageList->Add(*bin2c_nice_stopped_png);
                
                // Killed
                _imageList->Add(*bin2c_realtime_killed_png);
                _imageList->Add(*bin2c_not_nice_killed_png);
                _imageList->Add(*bin2c_normal_killed_png);
                _imageList->Add(*bin2c_nice_killed_png);

                // Zombie
                _imageList->Add(*bin2c_realtime_zombie_png);
                _imageList->Add(*bin2c_not_nice_zombie_png);
                _imageList->Add(*bin2c_normal_zombie_png);
                _imageList->Add(*bin2c_nice_zombie_png);

            }

            void ProcessTreeViewController::CreateProcessesTreeList()
            {

                if(_viewState == ViewState::Tree)
       			    _processesListControl = new Control::SearchableTreeListControl(this, wxID_ANY, _imageList);
                else if(_viewState == ViewState::List)
                    _processesListControl = new Control::SearchableListControl(this, wxID_ANY, _imageList);
    			
	    		_processesListControl->AppendColumn(_T("Name"), 300, wxALIGN_LEFT, wxCOL_RESIZABLE | wxCOL_SORTABLE);
		    	_processesListControl->AppendColumn(_T("PID"), 64, wxALIGN_RIGHT, wxCOL_RESIZABLE | wxCOL_SORTABLE);
                _processesListControl->AppendColumn(_T("CPU, %"), 88, wxALIGN_RIGHT, wxCOL_RESIZABLE | wxCOL_SORTABLE);
                _processesListControl->AppendColumn(_T("State"), 72, wxALIGN_LEFT, wxCOL_RESIZABLE | wxCOL_SORTABLE);
                _processesListControl->AppendColumn(_T("VM, Gib"), 84, wxALIGN_RIGHT, wxCOL_RESIZABLE | wxCOL_SORTABLE);
                _processesListControl->AppendColumn(_T("Resident, Mb"), 116, wxALIGN_RIGHT, wxCOL_RESIZABLE | wxCOL_SORTABLE);
                _processesListControl->AppendColumn(_T("Swapped, Mb"), 120, wxALIGN_RIGHT, wxCOL_RESIZABLE | wxCOL_SORTABLE);
                _processesListControl->AppendColumn(_T("Threads"), 88, wxALIGN_RIGHT, wxCOL_RESIZABLE | wxCOL_SORTABLE);
                _processesListControl->AppendColumn(_T("Priority"), 84, wxALIGN_RIGHT, wxCOL_RESIZABLE | wxCOL_SORTABLE);
                _processesListControl->AppendColumn(_T("Nice"), 64, wxALIGN_RIGHT, wxCOL_RESIZABLE | wxCOL_SORTABLE);
                _processesListControl->AppendColumn(_T("CPU, #"), 88, wxALIGN_RIGHT, wxCOL_RESIZABLE | wxCOL_SORTABLE);
                _processesListControl->AppendColumn(_T(""), 64, wxALIGN_RIGHT);

                
                //TODO: Uncomment this
			    _processesListControl->SetItemComparator(&_processesTreeListItemComparator);

                SetAutoCompleteChoices();

            }

            void ProcessTreeViewController::CreateMainSizer()
            {
                _bsSizer = new wxBoxSizer(wxVERTICAL);
                _bsSizer->Add(_processesListControl, 1, wxEXPAND | wxALL, 0);
                this->SetSizer(_bsSizer);
            }

            void ProcessTreeViewController::CreateContextMenu()
            {
                using SystemExplorer::Core::SignalManager;
                using SystemExplorer::Core::Models::Signal;
                using SystemExplorer::Core::Models::SignalType;

                _processContextMenu = new wxMenu();
                _processContextMenu->Append(static_cast<int>(ProcessContextMenuId::Open), wxT("&Open\tCtrl+O"), wxT("Open"));
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

                _processContextMenu->AppendSeparator();
                _processContextMenu->Append(static_cast<int>(ProcessContextMenuId::ExpandAll), wxT("&Expand All\tCtrl+E"), wxT("Expand All"));
                _processContextMenu->Append(static_cast<int>(ProcessContextMenuId::CollapseAll), wxT("&Collapse All\tCtrl+L"), wxT("Collapse All"));
                _processContextMenu->AppendSeparator();
                _processContextMenu->Append(static_cast<int>(ProcessContextMenuId::ToggleView), wxT("&Toggle Tree/List\tCtrl+T"), wxT("Expand All"));
                _processContextMenu->AppendSeparator();
                _processContextMenu->Append(static_cast<int>(ProcessContextMenuId::About), wxT("&About\tCtrl+Shift+A"), wxT("About"));
                _processContextMenu->AppendSeparator();
                _processContextMenu->Append(static_cast<int>(ProcessContextMenuId::Close), wxT("&Close\tAlt+F4"), wxT("Close"));

            }

            void ProcessTreeViewController::SetFocus()
            {
                _processesListControl->SetFocus();
            }

            void ProcessTreeViewController::BindEvents()
            {
                this->Bind(wxEVT_SIZE, &ProcessTreeViewController::window_OnSize, this);
                BindProcessesListControlEvents();
                BindContextMenuEvents();
                BindTimerEvents();
            }

            void ProcessTreeViewController::BindProcessesListControlEvents()
            {
                _processesListControl->Bind(custEVT_SEARCH, &ProcessTreeViewController::processesListControl_Search, this); 
                _processesListControl->Bind(custEVT_ITEM_CONTEXT_MENU, &ProcessTreeViewController::processesListControl_OnItemContextMenu, this);
                _processesListControl->Bind(wxEVT_MENU, &ProcessTreeViewController::processesContextMenu_OnMenuItem, this);

            }

            void ProcessTreeViewController::BindContextMenuEvents()
            {
                _processContextMenu->Bind(wxEVT_MENU_HIGHLIGHT, &ProcessTreeViewController::processesContextMenu_OnMenuHighlight, this);
                _processContextMenu->Bind(wxEVT_MENU_OPEN, &ProcessTreeViewController::processesContextMenu_OnMenuOpen, this);
                _processContextMenu->Bind(wxEVT_MENU_CLOSE, &ProcessTreeViewController::processesContextMenu_OnMenuClose, this);
            }

            void ProcessTreeViewController::BindTimerEvents()
            {
                _timer->Bind(wxEVT_TIMER, &ProcessTreeViewController::timer_OnTick, this);
            }


            void ProcessTreeViewController::window_OnSize(wxSizeEvent &event)
            {
                
                if(_statusBar == nullptr)
                {
                    event.Skip();
                    return;
                }


                wxRect rect_cpu;
                _statusBar->GetFieldRect(1, rect_cpu);
                _cpuStat->SetPosition(rect_cpu.GetPosition());
/*
                wxPoint point_cpu(rect_cpu.GetPosition());
                point_cpu.x += 14;
                point_cpu.y += 1;
                wxSize size_cpu(rect_cpu.GetSize());
                size_cpu.SetWidth(size_cpu.GetWidth() - 14);
                //rect.SetPosition(point);
                _gProgressBarCpu->SetPosition(point_cpu);

*/
                wxRect rect_ram;
                _statusBar->GetFieldRect(2, rect_ram);
                _ramStat->SetPosition(rect_ram.GetPosition());
/*
                wxPoint point_ram(rect_ram.GetPosition());
                point_ram.x += 14;
                point_ram.y += 1;
                wxSize size_ram(rect_ram.GetSize());
                size_ram.SetWidth(size_ram.GetWidth() - 14);
                //rect.SetPosition(point);
                _gProgressBarRss->SetPosition(point_ram);
*/
                event.Skip();
            }

            void ProcessTreeViewController::timer_OnTick(wxTimerEvent &event)
            {
                // Recalculate processes statistics like a CPU load, etc.
                _processesStatManager.Tick();
                // Rebind data to show new processes tree/stat
                ReBindData();
            }

            void ProcessTreeViewController::processesListControl_Search(wxCommandEvent &event)
            {
                BindData();
            }

            void ProcessTreeViewController::processesListControl_OnItemContextMenu(wxCommandEvent &event)
            {
                _processesListControl->PopupMenu(_processContextMenu);
            }

            void ProcessTreeViewController::processesContextMenu_OnMenuOpen(wxMenuEvent &event)
            {
            }
            void ProcessTreeViewController::processesContextMenu_OnMenuClose(wxMenuEvent &event)
            {
                if (event.GetMenu() == _processContextMenu)
                    SetHelpStatusText("");
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
                SetHelpStatusText(help);
            }

            void ProcessTreeViewController::ToggleViewState()
            {
                if(_viewState == ViewState::Tree)
                {
                    _viewState = ViewState::List;

                }
                else if(_viewState == ViewState::List)
                {
                    _viewState = ViewState::Tree;
                }
            
            }
            void ProcessTreeViewController::UpdateView()
            {
                StopTimer();

                std::string sarchText = _processesListControl->GetSearchText();

                this->_bsSizer->Clear(true);
                
                if(_viewState == ViewState::Tree)
                    CreateImageList();

                CreateProcessesTreeList();
                CreateAcceleratorTable();
                BindProcessesListControlEvents();
                SetFocus();
                
                _bsSizer->Add(_processesListControl, 1, wxEXPAND | wxALL, 0);
                _bsSizer->Layout();

                _processesListControl->SetSearchText(sarchText);

                BindData();

                StartTimer();
            }

            void ProcessTreeViewController::processesContextMenu_OnMenuItem(wxCommandEvent &event)
            {
                std::vector<Control::SearchableControlBase::SearchableItem> selectedItems;
                if (!_processesListControl->GetSelections(selectedItems))

                if(UpdateAutoCompleteChoices())
                {
                    SetAutoCompleteChoices();
                    _userConfig.Save();
                }
                

                int menuItemId = event.GetId();
                //std::cout << "OnMenuItem" << menuItemId << std::endl;
                AboutDialogViewController *dlgAbout;

                if ((menuItemId >= PROCESS_CONTEXT_MENU_ROOT_BASE) && (menuItemId < (PROCESS_CONTEXT_MENU_ROOT_BASE + PROCESS_CONTEXT_MENU_RANGE_SIZE)))
                {
                    switch (static_cast<ProcessContextMenuId>(menuItemId))
                    {
                    case ProcessContextMenuId::Open:
                        wxMessageBox("Open Process Details", "Not Implemented Yet.", wxOK | wxICON_INFORMATION, this->_book->GetParent());
                        break;
                    case ProcessContextMenuId::ExpandAll:
                        _processesListControl->ExpandAll();
                        break;
                    case ProcessContextMenuId::CollapseAll:
                        _processesListControl->CollapseAll();
                        break;
                    case ProcessContextMenuId::ToggleView:
                        //wxMessageBox("Toggle View", "Not Implemented Yet.", wxOK | wxICON_INFORMATION, this->_book->GetParent());
                        ToggleViewState();
                        UpdateView();
                        break;
                    case ProcessContextMenuId::About:
                        //wxMessageBox("About", "Not Implemented Yet.", wxOK | wxICON_INFORMATION, this->_book->GetParent());
                        dlgAbout = new AboutDialogViewController(GetTopFrame());
                        dlgAbout->ShowModal();

                        break;
                    case ProcessContextMenuId::Close:
                        GetTopFrame()->Close(true);
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
                std::vector<Control::SearchableControlBase::SearchableItem> selectedItems;
                if (!_processesListControl->GetSelections(selectedItems))
                    return;

                std::for_each(selectedItems.begin(), selectedItems.end(),
                    [signal, this](Control::SearchableControlBase::SearchableItem const &selectedItem)
                    {
                        SystemExplorer::Core::SignalManager::SendSignal(selectedItem.GetId(), signal);
                    });
            }
            template <typename T>
            std::string to_string_with_precision(const T a_value, const int n = 6)
            {
                std::ostringstream out;
                out.precision(n);
                out << std::fixed << a_value;
                return std::move(out).str();
            }

            void ProcessTreeViewController::BindData_Before()
            {
            }
            void ProcessTreeViewController::BindData()
            {

                using SystemExplorer::Core::ProcessManager;
                using SystemExplorer::Core::Models::Process;
                using SystemExplorer::Core::Models::ProcessTree;
                using SystemExplorer::Core::Models::ProcessesStat;
                
                ProcessesStat processesStat = _processesStatManager.GetProcessesStat();
                ProcessManager pm;
                std::string searchFilter = _processesListControl->GetSearchText();
                ProcessTree processes = pm.GetProcessTree(searchFilter);

                std::vector<Control::SearchableControlBase::SearchableItem> items;

                for (std::map<pid_t, Process>::const_iterator it = processes.processes.begin(); it != processes.processes.end(); ++it)
                {

                    // TODO: Fill items with data
                    pid_t pid = (*it).first;
                    pid_t parentPid = (*it).second.GetParentPid();
                    std::string text = (*it).second.GetName();
                    bool matched = (*it).second.GetNameMatched();
                    bool picked = (*it).second.GetPicked();
                    float cpu_load = -1.0f;
                    if(processesStat.processes_stat.find(pid) != processesStat.processes_stat.end())
                        cpu_load = processesStat.processes_stat[pid].cpu_stat.cpu_usage_per_all_cores;

                    std::vector<std::string> other({std::to_string(pid), 
                        to_string_with_precision((cpu_load < 0.0f)?0.0f:cpu_load, 2),
                        to_string(processesStat.processes_stat[pid].state),
                        to_string_with_precision(processesStat.processes_stat[pid].mem_vsize, 2),
                        to_string_with_precision(processesStat.processes_stat[pid].mem_rss, 2),
                        to_string_with_precision(processesStat.processes_stat[pid].mem_pages_swapped, 2),
                        std::to_string(processesStat.processes_stat[pid].threads),
                        std::to_string(processesStat.processes_stat[pid].priority),
                        std::to_string(processesStat.processes_stat[pid].nice),
                        std::to_string(processesStat.processes_stat[pid].processor)});

                    int iconIndex = MapProcessStatToIconIndex(processesStat.processes_stat[pid]);

                    if (picked)
                    {
                        Control::SearchableControlBase::SearchableItem item(pid, text, parentPid, matched, other, iconIndex);
                        items.push_back(item);
                    }
                }

                _processesListControl->BindData(items);
                _processesListControl->ExpandAll();

                UpdateStatusBarStatistics(processesStat);
            }


            void ProcessTreeViewController::ReBindData()
            {
                using SystemExplorer::Core::ProcessManager;
                using SystemExplorer::Core::Models::Process;
                using SystemExplorer::Core::Models::ProcessTree;
                using SystemExplorer::Core::Models::ProcessesStat;

                // Get processes
                ProcessManager pm;
                ProcessesStat processesStat = _processesStatManager.GetProcessesStat();
                float x = 0.0;

                std::string searchFilter = _processesListControl->GetSearchText();
                ProcessTree processTreeToRebind = pm.GetProcessTree(searchFilter);
              

                std::vector<Control::SearchableControlBase::SearchableItem> items;
                for (std::map<pid_t, Process>::const_iterator it = processTreeToRebind.processes.begin(); it != processTreeToRebind.processes.end(); ++it)
                {
                    // TODO: Fill items with data
                    pid_t pid = (*it).first;
                    pid_t parentPid = (*it).second.GetParentPid();
                    std::string text = (*it).second.GetName();
                    bool matched = (*it).second.GetNameMatched();
                    bool picked = (*it).second.GetPicked();

                    float cpu_load = -1.0f;
                    if(processesStat.processes_stat.find(pid) != processesStat.processes_stat.end())
                        cpu_load = processesStat.processes_stat[pid].cpu_stat.cpu_usage_per_all_cores;

                    std::vector<std::string> other({std::to_string(pid), 
                        to_string_with_precision(((cpu_load < 0) ? 0 : cpu_load), 2),
                        to_string(processesStat.processes_stat[pid].state),
                        to_string_with_precision(processesStat.processes_stat[pid].mem_vsize, 2),
                        to_string_with_precision(processesStat.processes_stat[pid].mem_rss, 2),
                        to_string_with_precision(processesStat.processes_stat[pid].mem_pages_swapped, 2),
                        std::to_string(processesStat.processes_stat[pid].threads),
                        std::to_string(processesStat.processes_stat[pid].priority),
                        std::to_string(processesStat.processes_stat[pid].nice),
                        std::to_string(processesStat.processes_stat[pid].processor)});

                    int iconIndex = MapProcessStatToIconIndex(processesStat.processes_stat[pid]);

                    if (picked)
                    {
                        Control::SearchableControlBase::SearchableItem item(pid, text, parentPid, matched, other, iconIndex);
                        items.push_back(item);
                    }
                }

                _processesListControl->ReBindData(items);
                UpdateStatusBarStatistics(processesStat);
    //			_processesListControl->ExpandAll();
            }

            void ProcessTreeViewController::UpdateStatusBarStatistics(Core::Models::ProcessesStat const &processesStat)
            {
                if(_sbStatIndex < 0)
                    return;

                wxStatusBar *statusBar = GetStatusBar();
                if(statusBar == nullptr)
                    return;

                float selected_total_cpu = 0.0f;
                float selected_total_rss = 0.0f;

                std::vector<Control::SearchableControlBase::SearchableItem> selectedItems;
                if(_processesListControl->GetSelections(selectedItems))
                {
                    std::for_each(selectedItems.begin(), selectedItems.end(), 
                        [&processesStat, &selected_total_cpu, &selected_total_rss, this]
                            (Control::SearchableControlBase::SearchableItem const &searchableItem)
                        {
                            pid_t pid = searchableItem.GetId();

                            if(processesStat.processes_stat.find(pid) 
                                != processesStat.processes_stat.end())
                            {
                                selected_total_cpu += 
                                    processesStat.processes_stat.at(pid).cpu_stat.cpu_usage_per_all_cores;
                                selected_total_rss += 
                                    processesStat.processes_stat.at(pid).mem_rss;
                            }
                        });
                }

                // Gib
                selected_total_rss /= 1024;

                
                //GetSelectedItemsTotalStat();

                std::ostringstream text_cpu;
                text_cpu << "CPU: " << std::setprecision(2) << std::fixed << std::setw(6) 
                    << processesStat.processes_stat_common.cpu_load
                    << "(" << selected_total_cpu << ")";
                //    << " %";
                /*
                _gProgressBarCpu->SetValue(processesStat.processes_stat_common.cpu_load * 100);
                _gProgressBarCpu->Update();
                _gProgressBarCpu->UpdateWindowUI();
                //statusBar->SetStatusText(text_cpu.str(), _sbStatIndex);
                */
                std::ostringstream text_rss;
                text_rss << "RAM: " << std::setprecision(2) << std::fixed << std::setw(6) 
                    << processesStat.processes_stat_common.rss 
                    << "(" << selected_total_rss << ")"
                    << " Gib";
                /*
                _gProgressBarRss->SetValue(processesStat.processes_stat_common.rss / 32.0f * 100);
                _gProgressBarRss->Update();
                _gProgressBarRss->UpdateWindowUI();
                //statusBar->SetStatusText(text_rss.str(), _sbStatIndex + 1);
                */
                _cpuStat->SetText(text_cpu.str());
                _ramStat->SetText(text_rss.str()); 

            }

            int ProcessTreeViewController::MapProcessStatToIconIndex(Core::Models::ProcessStat processStat)
            {
                const int ICON_INDEX_REALTIME = 0;
                const int ICON_INDEX_NOT_NICE = 1;
                const int ICON_INDEX_NORMAL = 2;
                const int ICON_INDEX_NICE = 3;

                std::map<Core::Models::ProcessState, int> processStateMap(
                    {
                        {Core::Models::ProcessState::Sleeping, 1},
                        {Core::Models::ProcessState::Waiting, 2},
                        {Core::Models::ProcessState::Idle, 3},
                        {Core::Models::ProcessState::Running, 4},
                        {Core::Models::ProcessState::Stopped, 5},
                        {Core::Models::ProcessState::Dead1, 5},
                        {Core::Models::ProcessState::Dead2, 5},
                        {Core::Models::ProcessState::Zombie, 6}
                    });

                int STATE_INDEX = 0;
                if(processStateMap.find(processStat.state) != processStateMap.end())
                {
                    STATE_INDEX = processStateMap[processStat.state];
                }
                

                int result = ICON_INDEX_NORMAL;

                if(processStat.priority < 0)
                    result = ICON_INDEX_REALTIME;
                else if(processStat.priority >= 0 && processStat.priority < 20)
                    result = ICON_INDEX_NOT_NICE;
                else if(processStat.priority > 20)
                    result = ICON_INDEX_NICE;

                result += STATE_INDEX * 4;

                return result;
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
