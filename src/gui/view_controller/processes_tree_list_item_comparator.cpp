#include "processes_tree_list_item_comparator.hpp"

#include <iomanip>
#include <iostream>

namespace SystemExplorer
{
  namespace Gui
  {
    namespace ViewController
    {

      template<class T>
      int cmp(T a, T b)
      {
        if(a < b)
          return -1;
        else if(a > b)
          return 1;
        else
          return 0;
      }

      int ProcessesTreeListItemComparator::Compare (wxTreeListCtrl *treelist, unsigned column, wxTreeListItem first, wxTreeListItem second)
      {
        int result;

        if(column > 11)
          return 0;

        wxString 
          first_text = treelist->GetItemText(first, column),
          second_text = treelist->GetItemText(second, column);

        switch(column)
        {
        // Process name
          case 0:
          case 3:
          case 4:
            result = first_text.CmpNoCase(second_text);
            if(result != 0)
              return result;
            else
              return cmp(
                atoi(treelist->GetItemText(first, 1)), 
                atoi(treelist->GetItemText(second, 1)));
          case 1:
          case 8:
          case 9:
          case 10:
          case 11:
            if(atoi(first_text.c_str()) < atoi(second_text.c_str()))
              return  -1;
            else if(atoi(first_text.c_str()) > atoi(second_text.c_str()))
              return  1;
            else
              return cmp(
                atoi(treelist->GetItemText(first, 1)), 
                atoi(treelist->GetItemText(second, 1)));
          case 2:
          case 5:
          case 6:
          case 7:
              if(std::atof(first_text.c_str()) < std::atof(second_text.c_str()))
                return -1;
              else if(std::atof(first_text.c_str()) > std::atof(second_text.c_str()))
                return 1;
              else
                return cmp(
                  atoi(treelist->GetItemText(first, 1)), 
                  atoi(treelist->GetItemText(second, 1)));;
        }

        return first_text.CmpNoCase(second_text);
      }
    }
  }
}
