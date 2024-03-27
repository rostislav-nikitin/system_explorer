#include "searchable_treelist_control_base.hpp"
#include <iostream>

namespace SystemExplorer
{
    namespace Gui
    {
        //wxDEFINE_EVENT(custEVT_SEARCH, wxCommandEvent);
        //wxDEFINE_EVENT(custEVT_SELECTION_CHANGED, wxCommandEvent);
        //wxDEFINE_EVENT(custEVT_ITEM_CONTEXT_MENU, wxCommandEvent);
        //wxDEFINE_EVENT(custEVT_MENU, wxCommandEvent);
        

        namespace Control
        {
            SearchableTreeListControlBase::SearchableTreeListControlBase(
                wxWindow *parent, wxWindowID id, wxImageList *imageList) 
                    : SearchableControlBase(parent, wxID_ANY, imageList)
            {
            }

            void SearchableTreeListControlBase::CreateChildControls()
            {
                SearchableControlBase::CreateChildControls();
                
		        _tlcTreeList = new wxTreeListCtrl(this, wxID_ANY, wxPoint(0,0), wxSize(100, 100), wxTL_MULTIPLE);
                _tlcTreeList->SetImageList(_imageList);
		        _tlcTreeList->SetWindowStyle(wxBORDER_NONE);

                //std::cout << (_tlcTreeList->GetParent() == nullptr) << std::endl;
                _bsSizer = new wxBoxSizer(wxVERTICAL);
		        _bsSizer->Add(_scSearch, 0, wxEXPAND | wxALL, 0);
		        _bsSizer->Add(_tlcTreeList, 1, wxEXPAND | wxALL, 0);
		        this->SetSizer(_bsSizer);
            }

            void SearchableTreeListControlBase::BindEvents()
            {
                SearchableControlBase::BindEvents();

                _tlcTreeList->Bind(wxEVT_CHAR, &SearchableTreeListControlBase::tlcTreeList_OnChar, this);
                _tlcTreeList->Bind(wxEVT_TREELIST_ITEM_CONTEXT_MENU, &SearchableTreeListControlBase::tlcTreeList_OnItemContextMenu, this);
            }

            int SearchableTreeListControlBase::AppendColumn(const wxString &title, int width, wxAlignment align, int flags)
            {
                return _tlcTreeList->AppendColumn(title, width, align, flags);
            }


            unsigned int SearchableTreeListControlBase::GetSelections(std::vector<SearchableItem> &selections) const
            {
                wxTreeListItems treeListItems;
                int result = _tlcTreeList->GetSelections(treeListItems);
                

                std::for_each(treeListItems.begin(), treeListItems.end(), 
                [&selections, this](wxTreeListItem const &treeListItem)
                {
                    SearchableItem *searchableItem = 
                        (SearchableItem *)_tlcTreeList->GetItemData(treeListItem);
                    selections.push_back(*searchableItem);
                });

                return selections.size();

            }

            
            void SearchableTreeListControlBase::SetItemComparator(wxTreeListItemComparator *treeListItemCopmarator)
            {
                _tlcTreeList->SetItemComparator(treeListItemCopmarator);
            }

            void SearchableTreeListControlBase::tlcTreeList_OnChar(wxKeyEvent &event)
            {
                	//TODO: Implement ALT-Letter to focus on search
	            int keyCode = event.GetKeyCode();
	            wxTreeListItems selectedItems;
	            //std::cout << keyCode << std::endl;

                
                if(event.IsKeyInCategory(WXK_CATEGORY_NAVIGATION))
                {
                    wxTreeListItem selectedItem = _tlcTreeList->GetSelection();
                    if(selectedItem.IsOk())
                    {
                        if(keyCode == WXK_LEFT)
                        {
                            _tlcTreeList->Collapse(selectedItem);
                        }
                        else if(keyCode == WXK_RIGHT)
                        {
                            _tlcTreeList->Expand(selectedItem);
                        }
                    }
                }

                if(keyCode == WXK_BACK)
                {
                    _scSearch->SetFocus();
            		_scSearch->SelectNone();

                }
                else if(keyCode == WXK_SPACE)
                {
                    _scSearch->SetValue("");
                    _scSearch->SetFocus();
            		_scSearch->SelectNone();

                }
                else if(!event.IsKeyInCategory(WXK_CATEGORY_NAVIGATION) 
                    && (keyCode != WXK_TAB) 
                    && (keyCode >= WXK_SPACE) 
                    && (std::isalnum(keyCode) || std::ispunct(keyCode)))
	            {
            		_scSearch->SetValue(wxString(event.GetUnicodeKey()));
                	_scSearch->SetFocus();
            		_scSearch->SelectNone();
                }

                event.Skip(); 
            }


            void SearchableTreeListControlBase::tlcTreeList_OnItemContextMenu(wxTreeListEvent &event)
            {
                wxCommandEvent menu_event(custEVT_ITEM_CONTEXT_MENU, GetId());

                menu_event.SetEventObject(this);
                menu_event.SetString("Menu Item!");

                ProcessWindowEvent(menu_event);
            }
/*
            void SearchableTreeListControlBase::tlcTreeList_OnMenuItem(wxCommandEvent &event)
            {
                std::cout << "STREELIST::ON MENU ITEM" << std::endl;
                wxCommandEvent menu_event(custEVT_MENU, GetId());

                menu_event.SetEventObject(this);
                menu_event.SetString("Menu Item!");

                ProcessWindowEvent(menu_event);
            }
*/
            void SearchableTreeListControlBase::OnSearchTextClick()
            {
                SearchableControlBase::OnSearchTextClick();
	            _tlcTreeList->SetFocus();
            }


            void SearchableTreeListControlBase::SetFocus()
            {
                _tlcTreeList->SetFocus();
                wxTreeListItem firstItem = _tlcTreeList->GetFirstItem();
                if (firstItem.IsOk())
                {
                    _tlcTreeList->Select(firstItem);
                }
            }

            void SearchableTreeListControlBase::ExpandAll()
            {
                wxTreeListItem itemToExpand = _tlcTreeList->GetRootItem();

                if(itemToExpand.IsOk())
                    ExpandAll(itemToExpand);
            }

            void SearchableTreeListControlBase::CollapseAll()
            {
                wxTreeListItem itemToExpand = _tlcTreeList->GetRootItem();

                if(itemToExpand.IsOk())
                    CollapseAll(itemToExpand);
            }

            void SearchableTreeListControlBase::PopupMenu(wxMenu *menu, const wxPoint &pos)
            {
                _tlcTreeList->PopupMenu(menu, pos);
            }


            // Private
            void SearchableTreeListControlBase::ExpandAll(wxTreeListItem &item)
            {
                std::vector<wxTreeListItem> nodes = GetAllSubNodes(item);
                std::for_each(nodes.begin(), nodes.end(), [this] (wxTreeListItem &item)
                {
                    _tlcTreeList->Expand(item);
                });
            }

            void SearchableTreeListControlBase::CollapseAll(wxTreeListItem &item)
            {
                std::vector<wxTreeListItem> nodes = GetAllSubNodes(item);
                std::for_each(nodes.begin(), nodes.end(), [this] (wxTreeListItem &item)
                {
                    _tlcTreeList->Collapse(item);
                });
            }

            std::vector<wxTreeListItem> SearchableTreeListControlBase::GetAllSubNodes(wxTreeListItem &parent)
            {
                std::vector<wxTreeListItem> result;

                if(!parent.IsOk())
                    return result;

                result.push_back(parent);
                size_t current_index = 0;
                while(current_index < result.size())
                {
                    for(wxTreeListItem current = _tlcTreeList->GetFirstChild(result[current_index]); 
                        current.IsOk(); 
                        current = _tlcTreeList->GetNextSibling(current))
                    {
                        if(_tlcTreeList->GetFirstChild(current).IsOk())
                        {
                            result.push_back(current);
                        }
                    }

                    ++current_index;
                }

                return result;
            }

            wxTreeListItem SearchableTreeListControlBase::FindItemById(int id)
            {
                wxTreeListItem result;

                for(wxTreeListItem current = _tlcTreeList->GetFirstItem(); current.IsOk(); current = _tlcTreeList->GetNextItem(current))
                {
                    wxClientData *itemData = _tlcTreeList->GetItemData(current);
                    SearchableItem *item = static_cast<SearchableItem *>(itemData);
                    int itemId = item->GetId();
                    if(itemId == id)
                    {
                        result = current;
                        break;
                    }
                }

                return result;
            }

        }
    }
}