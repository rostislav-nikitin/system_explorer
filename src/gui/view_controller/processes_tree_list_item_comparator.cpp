#include "processes_tree_list_item_comparator.hpp"

namespace SystemExplorer
{
  namespace Gui
  {
    namespace ViewController
    {
      int ProcessesTreeListItemComparator::Compare (wxTreeListCtrl *treelist, unsigned column, wxTreeListItem first, wxTreeListItem second)
      {
        int result;

        wxString 
          first_text = treelist->GetItemText(first, column),
          second_text = treelist->GetItemText(second, column);
      
        switch(column)
        {
        // Process name
          case 0:
            return first_text.CmpNoCase(second_text);
          case 1:
            return atoi(first_text.c_str()) < atoi(second_text.c_str());
        }

        return 0;
      }
    }
  }
}
