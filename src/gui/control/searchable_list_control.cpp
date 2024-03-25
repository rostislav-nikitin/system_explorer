#include "searchable_list_control.hpp"
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
            SearchableListControl::SearchableListControl(
                wxWindow *parent, wxWindowID id, wxImageList *imageList) 
                    : SearchableControlBase(parent, wxID_ANY, imageList)
            {
                Initialize();
            }

            void SearchableListControl::CreateChildControls()
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

            void SearchableListControl::BindEvents()
            {
                SearchableControlBase::BindEvents();

                _tlcTreeList->Bind(wxEVT_CHAR, &SearchableListControl::tlcTreeList_OnChar, this);
                _tlcTreeList->Bind(wxEVT_TREELIST_ITEM_CONTEXT_MENU, &SearchableListControl::tlcTreeList_OnItemContextMenu, this);
            }

            int SearchableListControl::AppendColumn(const wxString &title, int width, wxAlignment align, int flags)
            {
                return _tlcTreeList->AppendColumn(title, width, align, flags);
            }


            unsigned int SearchableListControl::GetSelections(std::vector<SearchableItem> &selections) const
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

            bool SearchableListControl::FilterEmpty() const
            {
                return GetSearchText().empty();
            }

            void SearchableListControl::BindData(std::vector<SearchableItem> &dataSource)
            {
               	_tlcTreeList->DeleteAllItems();

                for(std::vector<SearchableItem>::const_iterator it = dataSource.begin(); it != dataSource.end(); ++it)
                {
                    SearchableItem item = *it;
                    wxTreeListItem parent;
                    parent = _tlcTreeList->GetRootItem();

                    if(item.GetSelected() || FilterEmpty())
                    {
                        wxTreeListItem treeListItem = _tlcTreeList->AppendItem(parent, item.GetText());
                        
                        _tlcTreeList->SetItemData(treeListItem, new SearchableItem(item));
                        _tlcTreeList->SetItemImage(treeListItem, item.GetIconIndex(), item.GetIconIndex());
                        //std::cout << "ITEMINDEX=" << item.GetIconIndex() << std::endl;
                        for(int idx = 0; idx < item.GetOther().size(); ++idx)
                        {
                            _tlcTreeList->SetItemText(treeListItem, idx + 1, item.GetOther()[idx]);
                        }
                        //if(treeListItem.IsOk() && item.GetSelected())
                            //  _tlcTreeList->Select(treeListItem);
                       
                    }
            
                }

                for(std::vector<SearchableItem>::const_iterator it = dataSource.begin(); it != dataSource.end(); ++it)
                {
                    SearchableItem item = *it;

                    if(item.GetSelected())
                    {
                        wxTreeListItem treeListItem = FindItemById(item.GetId());
                        if(treeListItem.IsOk())
                            _tlcTreeList->Select(treeListItem);
                    }
                }

            }

            void SearchableListControl::ReBindData(std::vector<SearchableItem> &dataSource)
            {
                // Cases
                // 1. Item retured
                for(std::vector<SearchableItem>::const_iterator it = dataSource.begin(); it != dataSource.end(); ++it)
                {
                    SearchableItem item = *it;
                    std::string text = item.GetText();
                    pid_t id = item.GetId();
                    pid_t parentId = item.GetParentId();
                    bool selected = item.GetSelected();

                    wxTreeListItem treeListItem = FindItemById(id);
                    if(!treeListItem.IsOk())
                    {
                        //	1.1. Picekd and not exists -- create(item)
                        wxTreeListItem parent;
                        parent = _tlcTreeList->GetRootItem();

                        if(item.GetSelected() || FilterEmpty())
                        {
                            wxTreeListItem processTreeListItem = _tlcTreeList->AppendItem(parent, text);
                            _tlcTreeList->SetItemData(processTreeListItem, new SearchableItem(item));
                            _tlcTreeList->SetItemImage(processTreeListItem, item.GetIconIndex(), item.GetIconIndex());
                            for(int idx = 0; idx < item.GetOther().size(); ++idx)
                            {
                                _tlcTreeList->SetItemText(processTreeListItem, idx + 1, item.GetOther()[idx]);
                            }
                            if(selected)
                                _tlcTreeList->Select(processTreeListItem);

                            _tlcTreeList->Expand(processTreeListItem);
                        }
                        else
                        {
                            //log error
                        }
                                    
                    }
                    else
                    {
                        _tlcTreeList->SetItemImage(treeListItem, item.GetIconIndex(), item.GetIconIndex());
                        for(int idx = 0; idx < item.GetOther().size(); ++idx)
                        {
                            _tlcTreeList->SetItemText(treeListItem, idx + 1, item.GetOther()[idx]);
                        }
                        
                    }
                    
                }

                // 2. Item not returned
                // 	2.1. Exists in tree -- detele
                wxTreeListItem result;
                std::vector<int> toDelete;

                for(wxTreeListItem current = _tlcTreeList->GetFirstItem(); current.IsOk(); current = _tlcTreeList->GetNextItem(current))
                {
                    wxString nodePid = _tlcTreeList->GetItemText(current, 1);
                    int id = atoi(nodePid.c_str());

                    auto item = std::find_if(dataSource.begin(), dataSource.end(), 
                        [id](SearchableItem const &item){ return item.GetId() == id;  });

                    if(item == dataSource.end())
                    {
                        toDelete.push_back(id);					
                    }
                }

                for(std::vector<int>::iterator it = toDelete.begin(); it != toDelete.end(); ++it)
                {
                    wxTreeListItem item = FindItemById(*it);
                    if(item.IsOk())
                    {
                        _tlcTreeList->DeleteItem(item);
                    }
                }
            }                        

            void SearchableListControl::SetItemComparator(wxTreeListItemComparator *treeListItemCopmarator)
            {
                _tlcTreeList->SetItemComparator(treeListItemCopmarator);
            }

            void SearchableListControl::tlcTreeList_OnChar(wxKeyEvent &event)
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
	            if(!event.IsKeyInCategory(WXK_CATEGORY_NAVIGATION) 
                    && (keyCode != WXK_TAB) 
                    && (keyCode >= WXK_SPACE) && (std::isalnum(keyCode) || std::ispunct(keyCode)))
	            {
            		_scSearch->SetValue(wxString(event.GetUnicodeKey()));
                	_scSearch->SetFocus();
            		_scSearch->SelectNone();
                }

                event.Skip(); 
            }


            void SearchableListControl::tlcTreeList_OnItemContextMenu(wxTreeListEvent &event)
            {
                wxCommandEvent menu_event(custEVT_ITEM_CONTEXT_MENU, GetId());

                menu_event.SetEventObject(this);
                menu_event.SetString("Menu Item!");

                ProcessWindowEvent(menu_event);
            }
/*
            void SearchableListControl::tlcTreeList_OnMenuItem(wxCommandEvent &event)
            {
                std::cout << "STREELIST::ON MENU ITEM" << std::endl;
                wxCommandEvent menu_event(custEVT_MENU, GetId());

                menu_event.SetEventObject(this);
                menu_event.SetString("Menu Item!");

                ProcessWindowEvent(menu_event);
            }
*/
            void SearchableListControl::OnSearchTextClick()
            {
                SearchableControlBase::OnSearchTextClick();
	            _tlcTreeList->SetFocus();
            }


            void SearchableListControl::SetFocus()
            {
                _tlcTreeList->SetFocus();
                wxTreeListItem firstItem = _tlcTreeList->GetFirstItem();
                if (firstItem.IsOk())
                {
                    _tlcTreeList->Select(firstItem);
                }
            }

            void SearchableListControl::ExpandAll()
            {
                wxTreeListItem itemToExpand = _tlcTreeList->GetRootItem();

                if(itemToExpand.IsOk())
                    ExpandAll(itemToExpand);
            }

            void SearchableListControl::CollapseAll()
            {
                wxTreeListItem itemToExpand = _tlcTreeList->GetRootItem();

                if(itemToExpand.IsOk())
                    CollapseAll(itemToExpand);
            }

            void SearchableListControl::PopupMenu(wxMenu *menu, const wxPoint &pos)
            {
                _tlcTreeList->PopupMenu(menu, pos);
            }


            // Private
            void SearchableListControl::ExpandAll(wxTreeListItem &item)
            {
                std::vector<wxTreeListItem> nodes = GetAllSubNodes(item);
                std::for_each(nodes.begin(), nodes.end(), [this] (wxTreeListItem &item)
                {
                    _tlcTreeList->Expand(item);
                });
            }

            void SearchableListControl::CollapseAll(wxTreeListItem &item)
            {
                std::vector<wxTreeListItem> nodes = GetAllSubNodes(item);
                std::for_each(nodes.begin(), nodes.end(), [this] (wxTreeListItem &item)
                {
                    _tlcTreeList->Collapse(item);
                });
            }

            std::vector<wxTreeListItem> SearchableListControl::GetAllSubNodes(wxTreeListItem &parent)
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

            wxTreeListItem SearchableListControl::FindItemById(int id)
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