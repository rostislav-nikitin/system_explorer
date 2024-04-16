#ifndef PROCESS_TREE_VIEW_CONTROLLER_HPP
#define PROCESS_TREE_VIEW_CONTROLLER_HPP

#include <string>
#include <vector>
#include <map>
#include <set>
#include <iterator>
#include <optional>
#include <iomanip>
#include <istream>
#include <fstream>
#include <sstream>
#include <optional>

#include <unistd.h>

#include <wx/wx.h>
#include <wx/accel.h>
#include <wx/bookctrl.h>
#include <wx/cshelp.h>
#include <wx/timer.h>
#include <wx/imaglist.h>

#include <fs/fs.hpp>
#include <os/process/process_manager.hpp>
#include <os/signal/signal_manager.hpp>
#include <os/stat/processes_stat_manager.hpp>
#include <os/stat/process_state_helpers.hpp>
#include <os/stat/model/process_stat.hpp>
#include <os/stat/model/process_state.hpp>


#include <ui/control/icon_control.hpp>
#include <ui/control/searchable_control_base.hpp>
#include <ui/control/searchable_list_control.hpp>
#include <ui/control/searchable_treelist_control.hpp>
#include <ui/control/searchable_listview_control.hpp>


#include "../../config/user_config.hpp"

//#include "../../core/process_manager.hpp"
//#include "../../core/processes_stat_manager.hpp"
//#include "../../core/signal_manager.hpp"
//#include "../../core/models/process_stat.hpp"
//#include "../../core/models/process_state.hpp"
//#include "../../core/models/process_state_helpers.hpp"

//#include "../control/icon_control.hpp"
//#include "../control/searchable_control_base.hpp"
//#include "../control/searchable_list_control.hpp"
//#include "../control/searchable_treelist_control.hpp"
//#include "../control/searchable_listview_control.hpp"

#include "view_controller_base.hpp"
#include "about_dialog_view_controller.hpp"



namespace SystemExplorer
{
    namespace Gui
    {
        namespace ViewController
        {
            class ProcessTreeViewController : public ViewControllerBase
            {
            private:

                const char * RELATIVE_CONFIG_PATH = ".system_explorer";
                const char * PROCESSES_AUTOCOMPLETE_CHOICES_PATH = "processes_autocomplete_choices.dat";
            
	  		    const int REFRESH_INTERVAL = 1000; //ms

	            const int PROCESS_CONTEXT_MENU_RANGE_SIZE = 1000;

                const int PROCESS_CONTEXT_MENU_ROOT_BASE = 0;
                const int PROCESS_CONTEXT_MENU_SIGNAL_TYPE_BASE = 1000;
                const int PROCESS_CONTEXT_MENU_SIGNAL_BASE = 2000;

                enum class ProcessContextMenuId
                {
                    Open,
                    ExpandAll,
                    CollapseAll,
                    ToggleView,
                    AutoSort,
                    ShowAllProcesses,
                    About,
                    Close,
                    SendSignal,
                    KillSigTerm,
                    KillSigKill
                };

                enum class ViewState
                {
                    Tree,
                    List
                };


                Config::UserConfig &_userConfig;
                ViewState _viewState;

                wxStatusBar *_statusBar;
                int _sbStatIndex;
	            std::map<int, wxAcceleratorEntry> _hotKeys;

                wxImageList *_imageList;
                wxMenu *_processContextMenu;
	            wxTimer *_timer;
	            UI::Control::SearchableControlBase *_processesListControl;
                //Control::SearchableListControl *_processesListControl;

                OS::Process::ProcessManager _processManager;
                OS::Stat::ProcessesStatManager _processesStatManager;

                wxBoxSizer *_bsSizer;

                UI::Control::IconControl *_cpuStat;
                UI::Control::IconControl *_ramStat;
            
                wxGauge *_gProgressBarCpu;
                wxGauge *_gProgressBarRss;

                wxMenuItem *_miExpandAll;
                wxMenuItem *_miCollapseAll;
                wxMenuItem *_miToggleView;
                wxMenuItem *_miAutoSort;
                wxMenuItem *_miShowAllProcesses;

                void SetAutoCompleteChoices();
                bool UpdateAutoCompleteChoices();

                void CreateStatusBarField();
                void CreateHotKeys();
	            void CreateAcceleratorTable();
	            void CreateTimer();

                void CreateImageList();
	            void CreateProcessesTreeList();
                void CreateMainSizer();
                void CreateContextMenu();

                void BindProcessesListControlEvents();
                void BindContextMenuEvents();
                void BindTimerEvents();

                void UpdateContextMenu();

                void SetFocus();
                void StartTimer();
                void StopTimer();
                void ReBindData();

                void ToggleViewState();
                void UpdateView();

           	    void AddHotKey(int menuBase, int itemId, wxAcceleratorEntryFlags flags, int key);

        	    std::optional<wxAcceleratorEntry> GetHotKey(int menuBase, int itemId);
	            void AttachMenuItem(int menuItemId, wxMenuItem *menuItem);

               
                template<class T>
                wxMenuItem *AppendMenuItem(wxMenu *parentMenu, T dataItem, int base = 0, std::string overridenName = "", wxItemKind kind = wxITEM_NORMAL)
                {
                    int itemId = dataItem.GetId();
                    int menuItemId = base + itemId;
                    std::string itemName = overridenName.empty() ? dataItem.GetAlias() : overridenName;
                    std::optional<wxAcceleratorEntry> hotKey = GetHotKey(base, itemId);
                    if(hotKey.has_value())
                    {
                        itemName += ("\t" + hotKey.value().ToString());
                    }
                    
                    wxMenuItem *menuItem = parentMenu->Append(menuItemId, itemName, dataItem.GetDescription(), kind);

                    if(hotKey.has_value())
                    {
                    //		 AttachMenuItem(menuItemId, menuItem);		 
                        wxAcceleratorEntry &current = _hotKeys[menuItemId];
                        current.Set(current.GetFlags(), current.GetKeyCode(), current.GetCommand(), menuItem);
                    }

                    
                    return menuItem;
                }
                
                template<class T>
                wxMenuItem *AppendSubMenu(wxMenu *parentMenu, T dataItem, wxMenu *subMenu, int base = 0, std::string overridenName = "")
                {
                    return parentMenu->Append(base + dataItem.GetId(), overridenName.empty() ? dataItem.GetAlias() : overridenName, subMenu, dataItem.GetDescription());
                }	  

                pid_t ExtractPid(wxTreeListItem const &item) const;
                void SendSignalToSelectedProcesses(int signal) const;

                void UpdateStatusBarStatistics(OS::Stat::Model::ProcessesStat const &processesStat);

                int MapProcessStatToIconIndex(OS::Stat::Model::ProcessStat processStat);

                void Sort();
                
                void window_OnSize(wxSizeEvent& event);

                void processesListControl_Search(wxCommandEvent &event);
                void processesListControl_OnItemContextMenu(wxCommandEvent &event);

                void processesContextMenu_OnMenuItem(wxCommandEvent &event);
                void processesContextMenu_OnMenuHighlight(wxMenuEvent &event);
                void processesContextMenu_OnMenuOpen(wxMenuEvent &event);
                void processesContextMenu_OnMenuClose(wxMenuEvent &event);

                void timer_OnTick(wxTimerEvent &event);

            protected:
                virtual void CreateChildControls() override;
                virtual void BindEvents() override;
                virtual void BindData_Before() override;
                virtual void BindData() override;

                virtual void PostInitialize() override;

            public:
                ProcessTreeViewController(wxBookCtrl *book, 
                    OS::Process::ProcessManager processManager, 
                    std::string title, 
                    Config::UserConfig &userConfig,
                    wxWindowID id = wxID_ANY, 
                    bool useByDefault = false);
                ~ProcessTreeViewController();
            };
        }
    }
}

#endif