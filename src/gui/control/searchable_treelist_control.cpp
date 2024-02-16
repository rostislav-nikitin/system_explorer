#include "searchable_treelist_control.hpp"

namespace SystemExplorer
{
    namespace Gui
    {
        wxDEFINE_EVENT(custEVT_SEARCH, wxCommandEvent);
        wxDEFINE_EVENT(custEVT_SELECTION_CHANGED, wxCommandEvent);
        wxDEFINE_EVENT(custEVT_ITEM_CONTEXT_MENU, wxCommandEvent);

        namespace Control
        {
            SearchableTreeListControl::SearchableTreeListControl(wxWindowID id)
            {
                scSearch = new wxSearchCtrl(this, id, _T(""), wxPoint(0,0), wxSize(100, 32));
		        scSearch->SetDescriptiveText("Filter");

		        tlcTreeList = new wxTreeListCtrl(this, wxID_ANY, wxPoint(0,0), wxSize(100, 100), wxTL_MULTIPLE);
		        tlcTreeList->SetWindowStyle(wxBORDER_NONE);
		        //tlcTreeList->AppendColumn(_T("Name"),240, wxALIGN_LEFT, wxCOL_RESIZABLE | wxCOL_SORTABLE);
		        //tlcTreeList->AppendColumn(_T("PID"), 100, wxALIGN_LEFT, wxCOL_RESIZABLE | wxCOL_SORTABLE);
            }

            int SearchableTreeListControl::AppendColumn(const wxString &title, int width, wxAlignment align, int flags)
            {
                return tlcTreeList->AppendColumn(title, width, align, flags);
            }

            void SearchableTreeListControl::Bind(std::vector<SearchableTreeListItem> &dataSource)
            {
                scSearch->Bind(wxEVT_CHAR, &SearchableTreeListControl::tlcTreeList_OnChar, this);
	            scSearch->Bind(wxEVT_SEARCHCTRL_SEARCH_BTN, &SearchableTreeListControl::scSearch_Click, this);
	            scSearch->Bind(wxEVT_TEXT, &SearchableTreeListControl::scSearch_Text, this);
	            scSearch->Bind(wxEVT_KILL_FOCUS, &SearchableTreeListControl::scSearch_OnKillFocus, this);
                tlcTreeList->Bind(wxEVT_CHAR, &SearchableTreeListControl::tlcTreeList_OnChar, this);
	            tlcTreeList->Bind(wxEVT_TREELIST_SELECTION_CHANGED, &SearchableTreeListControl::tlcTreeList_OnSelectionChanged, this);
	            tlcTreeList->Bind(wxEVT_TREELIST_ITEM_CONTEXT_MENU, &SearchableTreeListControl::tlcTreeList_OnItemContextMenu, this);
            }

            void SearchableTreeListControl::ReBind(std::vector<SearchableTreeListItem> &dataSource)
            {

            }

            void SearchableTreeListControl::SetItemComparator(wxTreeListItemComparator *treeListItemCopmarator)
            {
                tlcTreeList->SetItemComparator(treeListItemCopmarator);
            }

            void SearchableTreeListControl::tlcTreeList_OnChar(wxKeyEvent &event)
            {
            }

            void SearchableTreeListControl::tlcTreeList_OnAny(wxEvent &event)
            {
            }

            void SearchableTreeListControl::tlcTreeList_OnSelectionChanged(wxTreeListEvent &event)
            {
            }

            void SearchableTreeListControl::tlcTreeList_OnItemContextMenu(wxTreeListEvent &event)
            {
            }

            void SearchableTreeListControl::scSearch_Text(wxCommandEvent &event)
            {
            }

            void SearchableTreeListControl::scSearch_Click(wxCommandEvent &event)
            {
            }

            void SearchableTreeListControl::scSearch_OnKillFocus(wxFocusEvent &event)
            {
            }
        }
    }
}