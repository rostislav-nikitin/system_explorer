#include "searchable_treelist_control.hpp"

namespace SystemExplorer
{
    namespace Gui
    {
        wxDEFINE_EVENT(custEVT_SEARCH, wxCommandEvent);
        wxDEFINE_EVENT(custEVT_SELECTION_CHANGED, wxCommandEvent);
        wxDEFINE_EVENT(custEVT_ITEM_CONTEXT_MENU, wxCommandEvent);
        wxDEFINE_EVENT(custEVT_MENU, wxCommandEvent);

        namespace Control
        {
            SearchableTreeListControl::SearchableTreeListItem::
                SearchableTreeListItem(int id, std::string text, int parentId, bool selected, std::vector<std::string> other)
                    :_id(id), _text(text), _parentId(parentId), _selected(selected), _other(other)
            {
        
            }
            int SearchableTreeListControl::SearchableTreeListItem::GetId() const
            {
                return _id;
            }
            int SearchableTreeListControl::SearchableTreeListItem::GetParentId() const
            {
                return _parentId;
            }
            std::string SearchableTreeListControl::SearchableTreeListItem::GetText() const
            {
                return _text;
            }
            bool SearchableTreeListControl::SearchableTreeListItem::GetSelected() const
            {
                return _selected;
            }
            std::vector<std::string> SearchableTreeListControl::SearchableTreeListItem::GetOther() const
            {
                return _other;
            }


            SearchableTreeListControl::SearchableTreeListControl(wxBookCtrl *parent, wxWindowID id) : wxPanel(parent)
            {
                _scSearch = new wxSearchCtrl(this, id, _T(""), wxPoint(0,0), wxSize(100, 32));
		        _scSearch->SetDescriptiveText("Filter");

		        _tlcTreeList = new wxTreeListCtrl(this, wxID_ANY, wxPoint(0,0), wxSize(100, 100), wxTL_MULTIPLE);
		        _tlcTreeList->SetWindowStyle(wxBORDER_NONE);

                _bsSizer = new wxBoxSizer(wxVERTICAL);
		        _bsSizer->Add(_scSearch, 0, wxEXPAND | wxALL, 0);
		        _bsSizer->Add(_tlcTreeList, 1, wxEXPAND | wxALL, 0);
		        this->SetSizer(_bsSizer);

                BindEvents();
            }

            void SearchableTreeListControl::BindEvents()
            {
	            _scSearch->Bind(wxEVT_SEARCHCTRL_SEARCH_BTN, &SearchableTreeListControl::scSearch_Click, this);
	            _scSearch->Bind(wxEVT_TEXT, &SearchableTreeListControl::scSearch_Text, this);
	            _scSearch->Bind(wxEVT_KILL_FOCUS, &SearchableTreeListControl::scSearch_OnKillFocus, this);
                _tlcTreeList->Bind(wxEVT_CHAR, &SearchableTreeListControl::tlcTreeList_OnChar, this);
	            _tlcTreeList->Bind(wxEVT_TREELIST_SELECTION_CHANGED, &SearchableTreeListControl::tlcTreeList_OnSelectionChanged, this);
                _tlcTreeList->Bind(wxEVT_TREELIST_ITEM_CONTEXT_MENU, &SearchableTreeListControl::tlcTreeList_OnItemContextMenu, this);
                //this->Bind(wxEVT_MENU, &SearchableTreeListControl::tlcTreeList_OnMenuItem, this);
            }

            int SearchableTreeListControl::AppendColumn(const wxString &title, int width, wxAlignment align, int flags)
            {
                return _tlcTreeList->AppendColumn(title, width, align, flags);
            }

            const wxString & SearchableTreeListControl::GetItemText(wxTreeListItem item, unsigned int col) const
            {
                return _tlcTreeList->GetItemText(item, col);
            }

            unsigned int SearchableTreeListControl::GetSelections(wxTreeListItems &selections) const
            {
                return _tlcTreeList->GetSelections(selections);
            }

            void SearchableTreeListControl::DataBind(std::vector<SearchableTreeListItem> &dataSource)
            {
               	_tlcTreeList->DeleteAllItems();

                for(std::vector<SearchableTreeListItem>::const_iterator it = dataSource.begin(); it != dataSource.end(); ++it)
                {
                    SearchableTreeListItem item = *it;
                    wxTreeListItem parent;
                    if(item.GetParentId() == 0)
                        parent = _tlcTreeList->GetRootItem();
                    else
                        parent = FindItemById(item.GetParentId());

                    if(parent.IsOk())
                    {
                        wxTreeListItem treeListItem = _tlcTreeList->AppendItem(parent, item.GetText());
                        _tlcTreeList->SetItemText(treeListItem, 1, std::to_string(item.GetId()));
                        if(item.GetSelected())
                            _tlcTreeList->Select(treeListItem);  
                    }
            
                }
            }

            void SearchableTreeListControl::DataReBind(std::vector<SearchableTreeListItem> &dataSource)
            {
                // Cases
                // 1. Item retured
                for(std::vector<SearchableTreeListItem>::const_iterator it = dataSource.begin(); it != dataSource.end(); ++it)
                {
                    SearchableTreeListItem item = *it;
                    std::string text = item.GetText();
                    pid_t id = item.GetId();
                    pid_t parentId = item.GetParentId();

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
                            _tlcTreeList->SetItemText(processTreeListItem, 1, std::to_string(id));
                            _tlcTreeList->Expand(processTreeListItem);
                        }
                        else
                        {
                        //log error
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
                        [id](SearchableTreeListItem const &item){ return item.GetId() == id;  });

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

            void SearchableTreeListControl::SetItemComparator(wxTreeListItemComparator *treeListItemCopmarator)
            {
                _tlcTreeList->SetItemComparator(treeListItemCopmarator);
            }

            void SearchableTreeListControl::tlcTreeList_OnChar(wxKeyEvent &event)
            {
                	//TODO: Implement ALT-Letter to focus on search
	            int keyCode = event.GetKeyCode();
	            wxTreeListItems selectedItems;
	            //std::cout << keyCode << std::endl;

	            if(!event.IsKeyInCategory(WXK_CATEGORY_NAVIGATION) 
                    && (keyCode != WXK_TAB) 
                    && (keyCode >= WXK_SPACE) && std::isalnum(keyCode))
	            {
            		_scSearch->SetValue(wxString(event.GetUnicodeKey()));
                	_scSearch->SetFocus();
            		_scSearch->SelectNone();
                }

                event.Skip(); 
            }

            void SearchableTreeListControl::tlcTreeList_OnAny(wxEvent &event)
            {
            }

            void SearchableTreeListControl::tlcTreeList_OnSelectionChanged(wxTreeListEvent &event)
            {
            }

            void SearchableTreeListControl::tlcTreeList_OnItemContextMenu(wxTreeListEvent &event)
            {
                wxCommandEvent menu_event(custEVT_ITEM_CONTEXT_MENU, GetId());

                menu_event.SetEventObject(this);
                menu_event.SetString("Menu Item!");

                ProcessWindowEvent(menu_event);
            }

            void SearchableTreeListControl::tlcTreeList_OnMenuItem(wxCommandEvent &event)
            {
                std::cout << "STREELIST::ON MENU ITEM" << std::endl;
                wxCommandEvent menu_event(custEVT_MENU, GetId());

                menu_event.SetEventObject(this);
                menu_event.SetString("Menu Item!");

                ProcessWindowEvent(menu_event);
            }

            void SearchableTreeListControl::scSearch_Text(wxCommandEvent &event)
            {
                wxCommandEvent search_event(custEVT_SEARCH, GetId());

                search_event.SetEventObject(this);
                search_event.SetString("Send Clicked!");

                ProcessWindowEvent(search_event);
            }

            void SearchableTreeListControl::scSearch_Click(wxCommandEvent &event)
            {
                _scSearch->SelectAll();
	            _tlcTreeList->SetFocus();
            }

            void SearchableTreeListControl::scSearch_OnKillFocus(wxFocusEvent &event)
            {
                _scSearch->SelectAll();
                event.Skip();
            }

            void SearchableTreeListControl::SetFocus()
            {
                _tlcTreeList->SetFocus();
                wxTreeListItem firstItem = _tlcTreeList->GetFirstItem();
                if (firstItem.IsOk())
                {
                    _tlcTreeList->Select(firstItem);
                }
            }

            void SearchableTreeListControl::ExpandAll()
            {
                wxTreeListItem itemToExpand = _tlcTreeList->GetRootItem();

                if(itemToExpand.IsOk())
                    ExpandAll(itemToExpand);
            }

            void SearchableTreeListControl::CollapseAll()
            {
                wxTreeListItem itemToExpand = _tlcTreeList->GetRootItem();

                if(itemToExpand.IsOk())
                    CollapseAll(itemToExpand);
            }

            void SearchableTreeListControl::PopupMenu(wxMenu *menu, const wxPoint &pos)
            {
                _tlcTreeList->PopupMenu(menu, pos);
            }


            // Private
            void SearchableTreeListControl::ExpandAll(wxTreeListItem &item)
            {
                std::vector<wxTreeListItem> nodes = GetAllSubNodes(item);
                std::for_each(nodes.begin(), nodes.end(), [this] (wxTreeListItem &item)
                {
                    _tlcTreeList->Expand(item);
                });
            }

            void SearchableTreeListControl::CollapseAll(wxTreeListItem &item)
            {
                std::vector<wxTreeListItem> nodes = GetAllSubNodes(item);
                std::for_each(nodes.begin(), nodes.end(), [this] (wxTreeListItem &item)
                {
                    _tlcTreeList->Collapse(item);
                });
            }

            std::vector<wxTreeListItem> SearchableTreeListControl::GetAllSubNodes(wxTreeListItem &parent)
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


            wxTreeListItem SearchableTreeListControl::FindItemById(int id)
            {
                wxTreeListItem result;

                for(wxTreeListItem current = _tlcTreeList->GetFirstItem(); current.IsOk(); current = _tlcTreeList->GetNextItem(current))
                {
                    wxString nodeId = _tlcTreeList->GetItemText(current, 1);
                    if(atoi(nodeId.c_str()) == id)
                    {
                        result = current;
                        break;
                    }
                }

                return result;
            }

            std::string SearchableTreeListControl::GetSearchText() const
            {
                return _scSearch->GetValue().ToStdString();
            }

        }
    }
}