#ifndef SEARCHABLE_CONTROL_BASE_HPP
#define SEARCHABLE_CONTROL_BASE_HPP

#include <string>
#include <vector>
#include <algorithm>

#include <wx/wx.h>
#include <wx/headercol.h>
#include <wx/bookctrl.h>
#include <wx/srchctrl.h>
#include <wx/imaglist.h>
#include <wx/treelist.h>

#include "control_base.hpp"

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
        wxDECLARE_EVENT(custEVT_ITEM_CONTEXT_MENU, wxCommandEvent);
        
        namespace Control
        {
            class SearchableControlBase : public ControlBase
            {
            protected:
                wxImageList *_imageList;
	            wxSearchCtrl *_scSearch;

                virtual void CreateChildControls() override;
                virtual void BindEvents() override;

                virtual void OnSearchTextClick();
                
                void scSearch_Text(wxCommandEvent &event);
                void scSearch_Click(wxCommandEvent &event);
                void scSearch_OnKillFocus(wxFocusEvent &event);

            public:
                class SearchableItem : public wxClientData 
                {
                    int _id;
                    int _parentId;
                    std::string _text;
                    bool _selected;
                    std::vector<std::string> _other;
                    int _iconIndex;
                    wxColour _bgColour;
               public:
                    SearchableItem(int id, std::string text, 
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

                SearchableControlBase(wxWindow *parent, wxWindowID Id, wxImageList *imageList = nullptr);

                virtual void BindData(std::vector<SearchableItem> &items) = 0;
                virtual void ReBindData(std::vector<SearchableItem> &items) = 0;

                virtual int AppendColumn(const wxString &title, int width=wxCOL_WIDTH_AUTOSIZE, wxAlignment align=wxALIGN_LEFT, int flags=wxCOL_RESIZABLE) = 0;
                virtual unsigned int GetSelections(std::vector<SearchableItem> &selections) const = 0;

                void AutoComplete(wxArrayString choices);

                virtual void SetFocus() = 0;
                virtual void PopupMenu(wxMenu *menu, const wxPoint &pos = wxDefaultPosition) = 0;

                virtual std::string GetSearchText() const;
                virtual void SetSearchText(std::string value);
                virtual bool FilterEmpty() const;

                virtual void ExpandAll() = 0;
                virtual void CollapseAll() = 0;

                virtual void SetItemComparator(wxTreeListItemComparator *treeListItemCopmarator);

            };
        }
    }
}

#endif