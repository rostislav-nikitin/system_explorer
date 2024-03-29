#ifndef SEARCHABLE_TREELIST_CONTROL_HPP
#define SEARCHABLE_TREELIST_CONTROL_HPP

#include <string>
#include <vector>
#include <algorithm>

#include <wx/wx.h>
#include <wx/bookctrl.h>
#include <wx/treelist.h>
#include <wx/srchctrl.h>
#include <wx/imaglist.h>

#include "searchable_control_base.hpp"
#include "searchable_treelist_control_base.hpp"
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
            class SearchableTreeListControl : public SearchableTreeListControlBase
            {
            public:
                SearchableTreeListControl(wxWindow *parent, wxWindowID Id, 
                    wxImageList *imageList = nullptr);
                virtual void BindData(std::vector<SearchableItem> &dataSource) override;
                virtual void ReBindData(std::vector<SearchableItem> &dataSource) override;
            };
        }
    }
}

#endif