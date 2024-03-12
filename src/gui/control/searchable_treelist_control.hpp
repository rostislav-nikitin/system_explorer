#ifndef SEARCHABLE_TREELIST_CONTROL_HPP
#define SEARCHABLE_TREELIST_CONTROL_HPP

#include <string>
#include <vector>

#include <wx/wx.h>
#include <wx/bookctrl.h>
#include <wx/treelist.h>
#include <wx/srchctrl.h>
#include <wx/imaglist.h>

#include <wild_card_expr/wild_card_expr.hpp>

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
        wxDECLARE_EVENT(custEVT_MENU, wxCommandEvent);
        
        namespace Control
        {
            class SearchableTreeListControl : public wxPanel
            {
                wxImageList *_imageList;

                wxBoxSizer *_bsSizer;
	            wxSearchCtrl *_scSearch;
	            wxTreeListCtrl *_tlcTreeList;

                void BindEvents();

                void tlcTreeList_OnChar(wxKeyEvent &event);
                void tlcTreeList_OnAny(wxEvent &event);
                void tlcTreeList_OnSelectionChanged(wxTreeListEvent &event);
                void tlcTreeList_OnItemContextMenu(wxTreeListEvent &event);
                void tlcTreeList_OnMenuItem(wxCommandEvent &event);
                void scSearch_Text(wxCommandEvent &event);
                void scSearch_Click(wxCommandEvent &event);
                void scSearch_OnKillFocus(wxFocusEvent &event);

                void ExpandAll(wxTreeListItem &item);
                void CollapseAll(wxTreeListItem &item);

                std::vector<wxTreeListItem> GetAllSubNodes(wxTreeListItem &parent);
                wxTreeListItem FindItemById(int id);

            public:
                class SearchableTreeListItem : public wxClientData 
                {
                    int _id;
                    int _parentId;
                    std::string _text;
                    bool _selected;
                    std::vector<std::string> _other;
                    int _iconIndex;
                    wxColour _bgColour;
               public:
                    SearchableTreeListItem(int id, std::string text, 
                        int parentId = 0, bool selected = false, 
                        std::vector<std::string> other = std::vector<std::string>(), 
                        int iconIndex = -1,
                        wxColour bgColour = wxTransparentColour);

                    int GetId() const;
                    int GetParentId() const;
                    std::string GetText() const;
                    bool GetSelected() const;
                    std::vector<std::string> GetOther() const;
                    int GetIconIndex() const;
                    wxColour GetBgColour() const;
                };

                SearchableTreeListControl(wxWindow *parent, wxWindowID Id, wxImageList *imageList = nullptr);

                int AppendColumn(const wxString &title, int width=wxCOL_WIDTH_AUTOSIZE, wxAlignment align=wxALIGN_LEFT, int flags=wxCOL_RESIZABLE);
                const wxString & GetItemText(wxTreeListItem item, unsigned int col = 0U) const;
                unsigned int GetSelections(wxTreeListItems &selections) const;

                void DataBind(std::vector<SearchableTreeListItem> &dataSource);
                void DataReBind(std::vector<SearchableTreeListItem> &dataSource);

                void SetFocus();
                void ExpandAll();
                void CollapseAll();

                void PopupMenu(wxMenu *menu, const wxPoint &pos = wxDefaultPosition);

                void SetItemComparator(wxTreeListItemComparator *treeListItemCopmarator);


                std::string GetSearchText() const;
            };
        }
    }
}

#endif