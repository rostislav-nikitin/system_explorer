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
#include <wce/wce.hpp>

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
                // The method that iniitialy and on change event binds the data to the data view control(wxTreeList).
                virtual void BindData(std::vector<SearchableItem> &dataSource) override;
                // The method that rebinds data to the data view control(wxTreeList) by timer.
                virtual void ReBindData(std::vector<SearchableItem> &dataSource) override;
            };
        }
    }
}

#endif