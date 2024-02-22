#ifndef MAIN_WINDOW_HPP
#define MAIN_WINDOW_HPP

#include <string>
#include <vector>
#include <map>
#include <optional>

#include <wx/wx.h>
#include <wx/accel.h>
#include <wx/bookctrl.h>
#include <wx/treelist.h>
#include <wx/srchctrl.h>
#include <wx/richtooltip.h>
#include <wx/cshelp.h>
#include <wx/timer.h>

#include "processes_tree_list_item_comparator.hpp"
#include "control/searchable_treelist_control.hpp"

namespace SystemExplorer
{
    namespace Gui
    {
        class MainWindow : public wxFrame
        {
	  		const char *TITLE = "System Explorer";
	  		const int REFRESH_INTERVAL = 1000; //ms

	  		int const PROCESSES_SEARCH_ID = 2;
		  int const PROCESSES_TREE_ID = 3;

	  wxTreeListItems _selectedItems;

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

	  ProcessesTreeListItemComparator _processesTreeListItemComparator;
	  
	  wxBookCtrl *mainBook;
	  wxPanel *deamonsTab;

	  wxMenu *processContextMenu;
	  wxTimer *timer;

	  Control::SearchableTreeListControl *searchableTreeList;

	  pid_t _selectedPid;

	  void CreateHotKeys();
	  void CreateAcceleratorTable();
	  void CreateTimer();
	  void CreateMainBook();
	  void CreateProcessesTab();
	  void CreateDeamonsTab();
	  void CreateStatus();
	  void SetFocus();

	  void BindEvents();
	  void BindData();
	  void ReBindData();
	  void StartTimer();
	  void StopTimer();

	  void AddHotKey(int menuBase, int itemId, wxAcceleratorEntryFlags flags, int key);
	  std::optional<wxAcceleratorEntry> GetHotKey(int menuBase, int itemId);
	  void AttachMenuItem(int menuItemId, wxMenuItem *menuItem);

	   wxTreeListItem FindItemByPid(pid_t pid);

	  void searchableTreeList_Search(wxCommandEvent &event);
	  void searchableTreeList_OnItemContextMenu(wxCommandEvent &event);
	  void processContextMenu_OnMenuItem(wxCommandEvent &event);
	  void timer_OnTick(wxTimerEvent &event);
	  void processesContextMenu_OnMenuHighlight(wxMenuEvent &event);
	  void processesContextMenu_OnMenuOpen(wxMenuEvent &event);
	  void processesContextMenu_OnMenuClose(wxMenuEvent &event);

	  pid_t ExtractPid(wxTreeListItem const &item) const;
	  void SendSignalToSelectedProcesses(int signal) const;

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

	public:
	  MainWindow();
        };
    }
}
#endif
