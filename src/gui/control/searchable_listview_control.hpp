#ifndef SEARCHABLE_LISTVIEW_CONTROL_HPP
#define SEARCHABLE_LISTVIEW_CONTROL_HPP

#include <string>
#include <vector>
#include <algorithm>

#include <wx/wx.h>
#include <wx/bookctrl.h>
#include <wx/listctrl.h>
#include <wx/srchctrl.h>
#include <wx/imaglist.h>

#include "searchable_control_base.hpp"
#include "searchable_control_base.hpp"
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
        namespace Control
        {
            class SearchableListViewControl : public SearchableControlBase
            {
                wxBoxSizer *_bsSizer;
	            wxListView *_lvListView;

                virtual void CreateChildControls() override;
                virtual void BindEvents() override;

                void lvProcesses_OnChar(wxKeyEvent &event);
                void lvProcesses_OnItemContextMenu(wxListEvent &event);
                void lvProcesses_OnMenuItem(wxCommandEvent &event);

                bool FilterEmpty() const;
                long FindItemById(int id);

            public:
                SearchableListViewControl(wxWindow *parent, wxWindowID Id, wxImageList *imageList = nullptr);

                virtual int AppendColumn(const wxString &title, int width=wxCOL_WIDTH_AUTOSIZE, wxAlignment align=wxALIGN_LEFT, int flags=wxCOL_RESIZABLE) override;
                virtual unsigned int GetSelections(std::vector<SearchableItem> &selections) const;

                virtual void BindData(std::vector<SearchableItem> &dataSource) override;
                virtual void ReBindData(std::vector<SearchableItem> &dataSource) override;

                virtual void OnSearchTextClick() override;
                virtual void SetFocus() override;

                void ExpandAll();
                void CollapseAll();

                void PopupMenu(wxMenu *menu, const wxPoint &pos = wxDefaultPosition);

                //void SetItemComparator(wxTreeListItemComparator *treeListItemCopmarator);
            };
        }
    }
}

#endif