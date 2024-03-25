#include "searchable_control_base.hpp"

namespace SystemExplorer
{
    namespace Gui
    {
        wxDEFINE_EVENT(custEVT_SEARCH, wxCommandEvent);
        wxDEFINE_EVENT(custEVT_ITEM_CONTEXT_MENU, wxCommandEvent);

        namespace Control
        {
            //SearchableControlItem
            SearchableControlBase::SearchableItem::SearchableItem(
                int id, std::string text, int parentId, bool selected, std::vector<std::string> other, int iconIndex, wxColour bgColour)
                    :_id(id), _text(text), _parentId(parentId), _selected(selected), 
                        _other(other), _iconIndex(iconIndex), _bgColour(bgColour)
            {
            }

            int SearchableControlBase::SearchableItem::GetId() const
            {
                return _id;
            }
            int SearchableControlBase::SearchableItem::GetParentId() const
            {
                return _parentId;
            }
            std::string SearchableControlBase::SearchableItem::GetText() const
            {
                return _text;
            }
            bool SearchableControlBase::SearchableItem::GetSelected() const
            {
                return _selected;
            }
            std::vector<std::string> SearchableControlBase::SearchableItem::GetOther() const
            {
                return _other;
            }
            int SearchableControlBase::SearchableItem::GetIconIndex() const
            {
                return _iconIndex;
            }
            wxColour SearchableControlBase::SearchableItem::GetBgColour() const
            {
                return _bgColour;
            }

            // SearchableControlBase
            SearchableControlBase::SearchableControlBase(
                wxWindow *parent, wxWindowID id, wxImageList *imageList) 
                    : ControlBase(parent), _imageList(imageList)
            {
                // Initialize() should be called in the leaf control;
            }

            void SearchableControlBase::CreateChildControls()
            {
                _scSearch = new wxSearchCtrl(this, wxID_ANY, _T(""), wxPoint(0,0), wxSize(100, 32));
		        _scSearch->SetDescriptiveText("Filter");
            }

            void SearchableControlBase::BindEvents()
            {
	            _scSearch->Bind(wxEVT_SEARCHCTRL_SEARCH_BTN, &SearchableControlBase::scSearch_Click, this);
	            _scSearch->Bind(wxEVT_TEXT, &SearchableControlBase::scSearch_Text, this);
	            _scSearch->Bind(wxEVT_KILL_FOCUS, &SearchableControlBase::scSearch_OnKillFocus, this);
            }

            //scSearch
            void SearchableControlBase::AutoComplete(wxArrayString choices)
            {
                _scSearch->AutoComplete(choices);
            }

            void SearchableControlBase::scSearch_Text(wxCommandEvent &event)
            {
                wxCommandEvent search_event(custEVT_SEARCH, GetId());

                search_event.SetEventObject(this);
                search_event.SetString("Send Clicked!");

                ProcessWindowEvent(search_event);
            }
            void SearchableControlBase::OnSearchTextClick()
            {
                _scSearch->SelectAll();
                //TODO: Override in ingerited control this method. Call:_dataViewControl->SetFocus();
            }

            void SearchableControlBase::scSearch_Click(wxCommandEvent &event)
            {
                OnSearchTextClick();
            }

            void SearchableControlBase::scSearch_OnKillFocus(wxFocusEvent &event)
            {
                _scSearch->SelectAll();
                
                event.Skip();
            }

            std::string SearchableControlBase::GetSearchText() const
            {
                return _scSearch->GetValue().ToStdString();
            }

            void SearchableControlBase::SetSearchText(std::string value)
            {
                _scSearch->SetValue(value);
            }

            void SearchableControlBase::SetItemComparator(wxTreeListItemComparator *treeListItemCopmarator)
            {
                //TODO: Not Implemented in base. Should be implemented in concrete control
            }

       }
    }
}