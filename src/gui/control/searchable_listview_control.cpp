#include "searchable_listview_control.hpp"
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
            SearchableListViewControl::SearchableListViewControl(
                wxWindow *parent, wxWindowID id, wxImageList *imageList) 
                    : SearchableControlBase(parent, wxID_ANY, imageList)
            {
                Initialize();
            }

            void SearchableListViewControl::CreateChildControls()
            {
                SearchableControlBase::CreateChildControls();
                
		        _lvListView = new wxListView(this, wxID_ANY, wxPoint(0,0), wxSize(100, 100), wxLC_REPORT | wxBORDER_NONE);
                //_lvListView->SetSingleStyle(wxLC_REPORT, false);
                //TODO: Uncomment
                _lvListView->AssignImageList(_imageList, wxIMAGE_LIST_SMALL);
		        //_lvListView->SetWindowStyle(wxBORDER_NONE);

                //std::cout << (_lvListView->GetParent() == nullptr) << std::endl;
                _bsSizer = new wxBoxSizer(wxVERTICAL);
		        _bsSizer->Add(_scSearch, 0, wxEXPAND | wxALL, 0);
		        _bsSizer->Add(_lvListView, 1, wxEXPAND | wxALL, 0);

		        this->SetSizer(_bsSizer);
            }

            void SearchableListViewControl::BindEvents()
            {
                SearchableControlBase::BindEvents();

                _lvListView->Bind(wxEVT_CHAR, &SearchableListViewControl::lvProcesses_OnChar, this);
                _lvListView->Bind(wxEVT_LIST_ITEM_RIGHT_CLICK, &SearchableListViewControl::lvProcesses_OnItemContextMenu, this);
            }

            int SearchableListViewControl::AppendColumn(const wxString &title, int width, wxAlignment align, int flags)
            {
                wxListColumnFormat format = wxListColumnFormat::wxLIST_FORMAT_LEFT;
                if(align == wxAlignment::wxALIGN_RIGHT)
                    format = wxListColumnFormat::wxLIST_FORMAT_RIGHT;

                return _lvListView->AppendColumn(title, format, width);
            }


            unsigned int SearchableListViewControl::GetSelections(std::vector<SearchableItem> &selections) const
            {

                for(long item_index = _lvListView->GetFirstSelected();
                    item_index != -1; item_index = _lvListView->GetNextSelected(item_index))
                    {

                        SearchableItem *searchableItem = 
                            (SearchableItem *)_lvListView->GetItemData(item_index);
                        selections.push_back(*searchableItem);
                    }

                return selections.size();

            }

            void SearchableListViewControl::BindData(std::vector<SearchableItem> &dataSource)
            {
               	_lvListView->DeleteAllItems();

                int insertIndex = 0;
                for(std::vector<SearchableItem>::const_iterator it = dataSource.begin(); it != dataSource.end(); ++it)
                {
                    SearchableItem item = *it;

                    if(item.GetSelected() || FilterEmpty())
                    {
                        wxListItem listItem;

                        //listItem.SetId(item.GetId());
                        //listItem.SetText(item.GetText());
                        //listItem.SetImage(item.GetIconIndex());
                        int listItemIndex = _lvListView->InsertItem(insertIndex++, item.GetText(), item.GetIconIndex());
                        SearchableItem *dataItem = new SearchableItem(item);
                        _lvListView->SetItemPtrData(listItemIndex, (wxUIntPtr)dataItem);
                        
                        for(int idx = 0; idx < item.GetOther().size(); ++idx)
                        {
                            _lvListView->SetItem(listItemIndex, idx + 1, item.GetOther()[idx]);
                        }
                        if(item.GetSelected())
                            _lvListView->Select(listItemIndex, true);
                    }
                }
/*
                for(std::vector<SearchableItem>::const_iterator it = dataSource.begin(); it != dataSource.end(); ++it)
                {
                    SearchableItem item = *it;

                    if(item.GetSelected())
                    {
                        long listItemIndex = FindItemById(item.GetId());
                        if(listItemIndex != -1)
                            _lvListView->Select(listItemIndex, true);
                    }
                }
*/
            }

            bool SearchableListViewControl::FilterEmpty() const
            {
                return GetSearchText().empty();
            }

            void SearchableListViewControl::ReBindData(std::vector<SearchableItem> &dataSource)
            {
                // Cases
                // 1. Item retured
                int insertIndex = _lvListView->GetItemCount();
                for(std::vector<SearchableItem>::const_iterator it = dataSource.begin(); it != dataSource.end(); ++it)
                {
                    SearchableItem item = *it;
                    if(item.GetSelected() || FilterEmpty())
                    {
                        std::string text = item.GetText();
                        pid_t id = item.GetId();
                        pid_t parentId = item.GetParentId();
                        bool selected = item.GetSelected();

                        long listItemIndex = FindItemById(id);
                        if(listItemIndex == -1)
                        {
                            int listItemIndex = _lvListView->InsertItem(insertIndex++, item.GetText(), item.GetIconIndex());
                            SearchableItem *dataItem = new SearchableItem(item);
                            _lvListView->SetItemPtrData(listItemIndex, (wxUIntPtr)dataItem);
                    
                            for(int idx = 0; idx < item.GetOther().size(); ++idx)
                            {
                                _lvListView->SetItem(listItemIndex, idx + 1, item.GetOther()[idx]);
                            }

                            if(selected)
                                _lvListView->Select(listItemIndex);
                        }
                        else
                        {
                            _lvListView->SetItemImage (listItemIndex, item.GetIconIndex(), item.GetIconIndex());

                            for(int idx = 0; idx < item.GetOther().size(); ++idx)
                            {
                                _lvListView->SetItem(listItemIndex, idx + 1, item.GetOther()[idx]);
                            }
                            
                        }
                    }
                }

                // 2. Item not returned
                // 	2.1. Exists in tree -- detele
                std::vector<int> toDelete;

                for(
                    long current = _lvListView->GetNextItem(-1, wxLIST_NEXT_ALL); 
                    current != -1;
                    current = _lvListView->GetNextItem(current, wxLIST_NEXT_ALL))
                {
                    SearchableItem *searchableItem = (SearchableItem *)_lvListView->GetItemData(current);
                    int id = searchableItem->GetId();

                    auto item = std::find_if(dataSource.begin(), dataSource.end(), 
                        [id](SearchableItem const &item){ return item.GetId() == id;  });

                    if(item == dataSource.end() || (!(*item).GetSelected() && !FilterEmpty()))
                    {
                        toDelete.push_back(id);					
                    }
                }

                for(std::vector<int>::iterator it = toDelete.begin(); it != toDelete.end(); ++it)
                {
                    long listItemIndex = FindItemById(*it);
                    if(listItemIndex != -1)
                    {
                        _lvListView->DeleteItem(listItemIndex);
                    }
                }
            }                        

            /*void SearchableListViewControl::SetItemComparator(wxTreeListItemComparator *treeListItemCopmarator)
            {
                _lvListView->SetItemComparator(treeListItemCopmarator);
            }*/

            void SearchableListViewControl::lvProcesses_OnChar(wxKeyEvent &event)
            {
                	//TODO: Implement ALT-Letter to focus on search
	            int keyCode = event.GetKeyCode();
	            //std::cout << keyCode << std::endl;

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


            void SearchableListViewControl::lvProcesses_OnItemContextMenu(wxListEvent &event)
            {
                wxCommandEvent menu_event(custEVT_ITEM_CONTEXT_MENU, GetId());

                menu_event.SetEventObject(this);
                menu_event.SetString("Menu Item!");

                ProcessWindowEvent(menu_event);
            }

            void SearchableListViewControl::OnSearchTextClick()
            {
                SearchableControlBase::OnSearchTextClick();
	            _lvListView->SetFocus();
            }


            void SearchableListViewControl::SetFocus()
            {
                _lvListView->SetFocus();
                long firstItem = _lvListView->GetNextItem(-1);
                if (firstItem != -1)
                {
                    _lvListView->Select(firstItem);
                }
            }

            void SearchableListViewControl::ExpandAll()
            {
            }

            void SearchableListViewControl::CollapseAll()
            {
            }

            void SearchableListViewControl::PopupMenu(wxMenu *menu, const wxPoint &pos)
            {
                _lvListView->PopupMenu(menu, pos);
            }

             void SearchableListViewControl::Sort()
             {
                
             }


            // Private

            long SearchableListViewControl::FindItemById(int id)
            {
                long result = -1;

                for(long current = _lvListView->GetNextItem(-1); current != -1; current = _lvListView->GetNextItem(current))
                {
                    SearchableItem *item = (SearchableItem *)_lvListView->GetItemData(current);
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