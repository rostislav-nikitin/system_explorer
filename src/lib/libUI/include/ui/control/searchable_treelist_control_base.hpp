#ifndef LIBUI_CONTROL_SEARCHABLE_TREELIST_CONTROL_BASE_HPP
#define LIBUI_CONTROL_SEARCHABLE_TREELIST_CONTROL_BASE_HPP

#include <string>
#include <vector>
#include <algorithm>

#include <wx/wx.h>
#include <wx/bookctrl.h>
#include <wx/treelist.h>
#include <wx/srchctrl.h>
#include <wx/imaglist.h>

#include "searchable_control_base.hpp"
#include <wce/wce.hpp>

// RESPONSIBILITIES:
//  1. Show process data in a treelist
//  2. Generate EVT_SEARCH events when user typing text in a search box
//  3. Generate EVT_CONTEXT_MENU when user right-click on treelist
//  All the Alt-Del, Ctrl-Del evets will be processed by the AccelTable on the parent view

namespace UI
{
    namespace Control
    {
        class SearchableTreeListControlBase : public SearchableControlBase
        {
        protected:
            wxBoxSizer *_bsSizer;
            wxTreeListCtrl *_tlcTreeList;

            unsigned _currentSortingColumn;
            bool _currentSortingOrder;

            virtual void CreateChildControls() override;
            virtual void BindEvents() override;

            void tlcTreeList_OnChar(wxKeyEvent &event);
            void tlcTreeList_OnItemContextMenu(wxTreeListEvent &event);
            void tlcTreeList_OnMenuItem(wxCommandEvent &event);

            void ExpandAll(wxTreeListItem &item);
            void CollapseAll(wxTreeListItem &item);

            std::vector<wxTreeListItem> GetAllSubNodes(wxTreeListItem &parent);
            wxTreeListItem FindItemById(int id);

        public:
            SearchableTreeListControlBase(wxWindow *parent, wxWindowID Id, wxImageList *imageList = nullptr);

            virtual int AppendColumn(const wxString &title, int width=wxCOL_WIDTH_AUTOSIZE, wxAlignment align=wxALIGN_LEFT, int flags=wxCOL_RESIZABLE) override;
            virtual unsigned int GetSelections(std::vector<SearchableItem> &selections) const;

            virtual void OnSearchTextClick() override;
            virtual void SetFocus() override;

            void ExpandAll();
            void CollapseAll();

            void PopupMenu(wxMenu *menu, const wxPoint &pos = wxDefaultPosition);

            virtual void SetItemComparator(wxTreeListItemComparator *treeListItemCopmarator) override;
            virtual void Sort() override;
        };
    }
}
#endif