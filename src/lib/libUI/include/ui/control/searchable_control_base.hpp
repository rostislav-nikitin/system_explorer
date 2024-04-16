#ifndef LIBUI_CONTROL_SEARCHABLE_CONTROL_BASE_HPP
#define LIBUI_CONTROL_SEARCHABLE_CONTROL_BASE_HPP

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

#include <wce/wce.hpp>

namespace UI
{
    // The event that raises when used changes the search text.
    wxDECLARE_EVENT(custEVT_SEARCH, wxCommandEvent);
    // The event that raises when user opening data view's context menu.
    wxDECLARE_EVENT(custEVT_ITEM_CONTEXT_MENU, wxCommandEvent);
    
    namespace Control
    {
        // The class that represents a generic searchable control.
        // When user enter some search phrase then contol generates a cusSEARCH_EVT.
        // After that the client code can hande this event and rebind the data inside a control's data view(tree/list/other depends on derived class impementation).
        class SearchableControlBase : public ControlBase
        {
        protected:
            wxImageList *_imageList;
            wxSearchCtrl *_scSearch;

            virtual void CreateChildControls() override;
            virtual void BindEvents() override;

            virtual void OnSearchTextClick();

        private:                
            void scSearch_Text(wxCommandEvent &event);
            void scSearch_Click(wxCommandEvent &event);
            void scSearch_OnKillFocus(wxFocusEvent &event);

        public:
            
            // The class that represents a sarchable item.
            class SearchableItem : public wxClientData 
            {
            private:
                int _id;
                int _parentId;
                std::string _text;
                bool _selected;
                std::vector<std::string> _other;
                int _iconIndex;
                wxColour _bgColour;
            public:
                // The constructor.
                SearchableItem(int id, std::string text, 
                    int parentId = 0, bool selected = false, 
                    std::vector<std::string> other = std::vector<std::string>(), 
                    int iconIndex = -1,
                    wxColour bgColour = wxTransparentColour);

                // The getter of the Id property.
                int GetId() const;
                // The getter of the ParentId property.
                int GetParentId() const;
                // The getter of the Text property.
                std::string GetText() const;
                // The getter of the Selected property.
                // The Selected property determines does item selected in the data view control or not.
                bool GetSelected() const;
                // The getter of the Other property.
                // The Other property represents list of the additional properties not specified in the current iterface but which should be shown on the data view (values for the extra columns).
                std::vector<std::string> GetOther() const;
                // The getter for the IconIndex property.
                // The IconIndex property represents a item's icon index inside a control's attached wxImageList instance.
                int GetIconIndex() const;
                // The getter for the BgColor property.
                // The BgColor property represents a background color of the item inside a data view control.
                wxColour GetBgColour() const;
            };

            typedef int (*Comparer)(unsigned int column, std::pair<int, wxString> a, std::pair<int, wxString> b);

            // The constructor.
            SearchableControlBase(wxWindow *parent, wxWindowID Id, wxImageList *imageList = nullptr);

            // The method that binds data to the data view child control.
            virtual void BindData(std::vector<SearchableItem> &items) = 0;
            // The method that iniitialy and on change event binds the data to the data view control(wxTreeList).
            virtual void ReBindData(std::vector<SearchableItem> &items) = 0;
            // The method that rebinds data to the data view control(wxTreeList) by timer.
            virtual int AppendColumn(const wxString &title, int width=wxCOL_WIDTH_AUTOSIZE, wxAlignment align=wxALIGN_LEFT, int flags=wxCOL_RESIZABLE) = 0;
            // The method that gets all the selected searchable items of the data view.
            virtual unsigned int GetSelections(std::vector<SearchableItem> &selections) const = 0;
            // The method that attaches previously used seach texts to the search textbox control.
            void AutoComplete(wxArrayString choices);
            // The method that sets the focus to the default elenent of this control.
            virtual void SetFocus() = 0;
            // The method that shows popup menu.
            virtual void PopupMenu(wxMenu *menu, const wxPoint &pos = wxDefaultPosition) = 0;
            // The getter for the SearchText property.
            virtual std::string GetSearchText() const;
            // The setter for the SearchText property.
            virtual void SetSearchText(std::string value);
            // The property that indicates that filter(SearchText) is empty.
            virtual bool FilterEmpty() const;

            // The method that should expand all the data items(in case if dataview is a tree based).
            virtual void ExpandAll() = 0;
            // The method that should collapse all the data items(in case if dataview is a tree based).
            virtual void CollapseAll() = 0;

            // The methods that sets item comparator(used for data items sorting).
            //virtual void SetItemComparator(wxTreeListItemComparator *treeListItemCopmarator);
            virtual void SetComparer(Comparer comparer) = 0;
            // The method that should apply sortig for the data view.
            virtual void Sort() = 0;
        };
    }
}

#endif