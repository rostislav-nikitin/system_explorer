#ifndef MAIN_WINDOW_HPP
#define MAIN_WINDOW_HPP

#include <wx/wx.h>
#include <wx/bookctrl.h>
#include <wx/treelist.h>
#include <wx/srchctrl.h>
#include <wx/richtooltip.h>
#include <wx/cshelp.h>
#include <wx/timer.h>

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

			wxSimpleHelpProvider *helpProvider;

            wxBookCtrl *mainBook;
            wxPanel *processesTab;
            wxPanel *deamonsTab;

            wxBoxSizer *processesTabSizer;
            wxSearchCtrl *processesSearch;
            wxTreeListCtrl *processesTreeList;
			wxMenu *processContextMenu;
			wxTimer *timer;

            pid_t _selectedPid;

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

            wxTreeListItem FindItemByPid(pid_t pid);

            void processesTreeList_OnChar(wxKeyEvent &event);
			void processesTreeList_OnSelectionChanged(wxTreeListEvent &event);
			void precessesTreeList_OnItemContextMenu(wxTreeListEvent &event);
			void processesTreeList_OnMenuItem(wxCommandEvent &event);
            void processesSearch_Text(wxCommandEvent &event);
            void processesSearch_Click(wxCommandEvent &event);
			void timer_OnTick(wxTimerEvent &event);
			void processesContextMenu_OnMenuHighlight(wxMenuEvent &event);
			void processesContextMenu_OnMenuOpen(wxMenuEvent &event);
			void processesContextMenu_OnMenuClose(wxMenuEvent &event);


			pid_t ExtractPid(wxTreeListItem const &item) const;
			void SendSignalToSelectedProcesses(int signal) const;
            
        public:
            MainWindow();
        };
    }
}
#endif
