#include "../../../include/ui/control/searchable_treelist_control.hpp"

#include <iostream>

namespace UI
{
    //wxDEFINE_EVENT(custEVT_SEARCH, wxCommandEvent);
    //wxDEFINE_EVENT(custEVT_SELECTION_CHANGED, wxCommandEvent);
    //wxDEFINE_EVENT(custEVT_ITEM_CONTEXT_MENU, wxCommandEvent);
    //wxDEFINE_EVENT(custEVT_MENU, wxCommandEvent);
    
    namespace Control
    {
        SearchableTreeListControl::SearchableTreeListControl(
            wxWindow *parent, wxWindowID id, wxImageList *imageList) 
                : SearchableTreeListControlBase(parent, wxID_ANY, imageList)
        {
            Initialize();
        }

        void SearchableTreeListControl::BindData(std::vector<SearchableItem> &dataSource)
        {
            _tlcTreeList->DeleteAllItems();

            for(std::vector<SearchableItem>::const_iterator it = dataSource.begin(); it != dataSource.end(); ++it)
            {
                SearchableItem item = *it;
                wxTreeListItem parent;
                if(item.GetParentId() == 0)
                    parent = _tlcTreeList->GetRootItem();
                else
                    parent = FindItemById(item.GetParentId());

                if(parent.IsOk())
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

        void SearchableTreeListControl::ReBindData(std::vector<SearchableItem> &dataSource)
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
                    if(parentId == 0)
                        parent = _tlcTreeList->GetRootItem();
                    else
                        parent = FindItemById(parentId);

                    if(parent.IsOk())
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
        }
}