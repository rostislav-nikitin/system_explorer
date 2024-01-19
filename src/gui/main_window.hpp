#ifndef MAIN_WINDOW_HPP
#define MAIN_WINDOW_HPP

#include <wx/wx.h>
#include <wx/bookctrl.h>
#include <wx/treelist.h>
#include <wx/srchctrl.h>
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

			enum class ProcessContextMenuId
			{
				Open,
				KillSigHup,
				KillSigKill
			};

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
            void processesSearch_Click(wxCommandEvent &event);
			void timer_OnTick(wxTimerEvent &event);
            
        public:
            MainWindow();
        };
    }
}
#endif
