#ifndef MAIN_WINDOW_HPP
#define MAIN_WINDOW_HPP

#include <wx/wx.h>
#include <wx/bookctrl.h>
#include <wx/treectrl.h>
#include <wx/srchctrl.h>

namespace SystemExplorer
{
    namespace Gui
    {
        class MainWindow : public wxFrame
        {
            const char *TITLE = "System Explorer";

            int const PROCESSES_SEARCH_ID = 2;
            int const PROCESSES_TREE_ID = 3;

            wxBookCtrl *mainBook;
            wxPanel *processesTab;
            wxPanel *deamonsTab;

            wxTreeCtrl *processesTree;

            void CreateMainBook();
            void CreateProcessesTab();
            void CreateDeamonsTab();
            void CreateStatus();

            void BindEvents();

            void processesTree_OnKeyDown(wxTreeEvent &event);
            void processesSearch_Click(wxCommandEvent &event);
            
        public:
            MainWindow();
        };
    }
}
#endif
