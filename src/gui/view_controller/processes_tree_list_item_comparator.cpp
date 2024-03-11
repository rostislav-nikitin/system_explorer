#include "processes_tree_list_item_comparator.hpp"

#include <iomanip>
#include <iostream>

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
          case 3:
            return first_text.CmpNoCase(second_text);

          case 1:
          case 7:
          case 8:
          case 9:
          case 10:
            if(atoi(first_text.c_str()) < atoi(second_text.c_str()))
              return  -1;
            else if(atoi(first_text.c_str()) > atoi(second_text.c_str()))
              return  1;
            else
              return 0;
          case 2:
          case 4:
          case 5:
          case 6:
              if(std::atof(first_text.c_str()) < std::atof(second_text.c_str()))
                return -1;
              else if(std::atof(first_text.c_str()) > std::atof(second_text.c_str()))
                return 1;
              else
                return 0;
        }

        return first_text.CmpNoCase(second_text);
      }
    }
  }
}
