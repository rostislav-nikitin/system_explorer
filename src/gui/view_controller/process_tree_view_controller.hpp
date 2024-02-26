#ifndef PROCESS_TREE_VIEW_CONTROLLER_HPP
#define PROCESS_TREE_VIEW_CONTROLLER_HPP

#include <string>
#include <vector>
#include <map>
#include <optional>

#include <wx/wx.h>
#include <wx/accel.h>
#include <wx/bookctrl.h>
#include <wx/cshelp.h>
#include <wx/timer.h>

#include "../../core/process_manager.hpp"
#include "../../core/signal_manager.hpp"

#include "../control/searchable_treelist_control.hpp"
#include "../processes_tree_list_item_comparator.hpp"

#include "view_controller_base.hpp"

#include "../../core/process_manager.hpp"

namespace SystemExplorer
{
    namespace Gui
    {
        namespace ViewController
        {
            class ProcessTreeViewController : public ViewControllerBase
            {
            private:
            
	  		    const int REFRESH_INTERVAL = 1000; //ms

	            const int PROCESS_CONTEXT_MENU_RANGE_SIZE = 1000;

                const int PROCESS_CONTEXT_MENU_ROOT_BASE = 0;
                const int PROCESS_CONTEXT_MENU_SIGNAL_TYPE_BASE = 1000;
                const int PROCESS_CONTEXT_MENU_SIGNAL_BASE = 2000;

                enum class ProcessContextMenuId
                {
                    Open,
                    SendSignal,
                    KillSigTerm,
                    KillSigKill
                };


	            std::map<int, wxAcceleratorEntry> _hotKeys;

                SystemExplorer::Gui::ProcessesTreeListItemComparator _processesTreeListItemComparator;
	  
                wxMenu *_processContextMenu;
	            wxTimer *_timer;
	            Control::SearchableTreeListControl *_searchableTreeList;

                SystemExplorer::Core::ProcessManager _processManager;

                wxBoxSizer *_bsSizer;

                void CreateHotKeys();
	            void CreateAcceleratorTable();
	            void CreateTimer();

	            void CreateProcessesTreeList();

                void SetFocus();
                void StartTimer();
                void StopTimer();
                void ReBindData();

           	    void AddHotKey(int menuBase, int itemId, wxAcceleratorEntryFlags flags, int key);
        	    std::optional<wxAcceleratorEntry> GetHotKey(int menuBase, int itemId);
	            void AttachMenuItem(int menuItemId, wxMenuItem *menuItem);
                template<class T>
                wxMenuItem *AppendMenuItem(wxMenu *parentMenu, T dataItem, int base = 0, std::string overridenName = "")
                {
                    int itemId = dataItem.GetId();
                    int menuItemId = base + itemId;
                    std::string itemName = overridenName.empty() ? dataItem.GetAlias() : overridenName;
                    std::optional<wxAcceleratorEntry> hotKey = GetHotKey(base, itemId);
                    if(hotKey.has_value())
                    {
                    itemName += ("\t" + hotKey.value().ToString());
                    }
                    
                    wxMenuItem *menuItem = parentMenu->Append(menuItemId, itemName, dataItem.GetDescription());

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


                void searchableTreeList_Search(wxCommandEvent &event);
                void searchableTreeList_OnItemContextMenu(wxCommandEvent &event);

                void processesContextMenu_OnMenuItem(wxCommandEvent &event);
                void processesContextMenu_OnMenuHighlight(wxMenuEvent &event);
                void processesContextMenu_OnMenuOpen(wxMenuEvent &event);
                void processesContextMenu_OnMenuClose(wxMenuEvent &event);

                void timer_OnTick(wxTimerEvent &event);

            protected:
                virtual void CreateChildControls() override;
                virtual void BindEvents() override;
                virtual void BindData() override;

                virtual void PostInitialize() override;

            public:
                ProcessTreeViewController(wxBookCtrl *book, SystemExplorer::Core::ProcessManager processManager, std::string title, wxWindowID id = wxID_ANY, bool useByDefault = false);
                ~ProcessTreeViewController();
            };
        }
    }
}

#endif