#ifndef SEARCHABLE_TREELIST_CONTROL_HPP
#define SEARCHABLE_TREELIST_CONTROL_HPP

#include <string>
#include <vector>

#include <wx/wx.h>
#include <wx/treelist.h>
#include <wx/srchctrl.h>

// RESPONSIBILITIES:
//  1. Show process data in a treelist
//  2. Generate EVT_SEARCH events when user typing text in a search box
//  3. Generate EVT_CONTEXT_MENU when user right-click on treelist
//  All the Alt-Del, Ctrl-Del evets will be processed by the AccelTable on the parent view

namespace SystemExplorer
{
    namespace Gui
    {
        wxDECLARE_EVENT(custEVT_SEARCH, wxCommandEvent);
        wxDECLARE_EVENT(custEVT_SELECTION_CHANGED, wxCommandEvent);
        wxDECLARE_EVENT(custEVT_ITEM_CONTEXT_MENU, wxCommandEvent);

        namespace Control
        {
            class SearchableTreeListControl : wxPanel
            {
                class SearchableTreeListItem
                {
                    int id;
                    int parentId;
                    std::string text;
                    std::vector<std::string> other;
                };

                wxBoxSizer *bsSizer;
	            wxSearchCtrl *scSearch;
	            wxTreeListCtrl *tlcTreeList;

                void BindEvents();

                void tlcTreeList_OnChar(wxKeyEvent &event);
                void tlcTreeList_OnAny(wxEvent &event);
                void tlcTreeList_OnSelectionChanged(wxTreeListEvent &event);
                void tlcTreeList_OnItemContextMenu(wxTreeListEvent &event);
                void scSearch_Text(wxCommandEvent &event);
                void scSearch_Click(wxCommandEvent &event);
                void scSearch_OnKillFocus(wxFocusEvent &event);
            public:
                SearchableTreeListControl(wxWindowID Id);

                int AppendColumn(const wxString &title, int width=wxCOL_WIDTH_AUTOSIZE, wxAlignment align=wxALIGN_LEFT, int flags=wxCOL_RESIZABLE);

                void Bind(std::vector<SearchableTreeListItem> &dataSource);
                void ReBind(std::vector<SearchableTreeListItem> &dataSource);

                void SetItemComparator(wxTreeListItemComparator *treeListItemCopmarator);
            };
        }
    }
}

#endif