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
            return first_text.CmpNoCase(second_text);
          case 2:
            //std::cout << atoi(first_text.c_str()) << "::" << atoi(second_text.c_str()) << std::endl;
            if(atoi(first_text.c_str()) < atoi(second_text.c_str()))
              return  -1;
            else if(atoi(first_text.c_str()) > atoi(second_text.c_str()))
              return  1;
            else
              return 0;

          case 1:
            //if((std::atof(first_text.c_str()) > 0.0f) || (std::atof(second_text.c_str()) > 0.0f))
              //std::cout << std::setprecision(2) << "F" << std::atof(first_text.c_str()) << "::" << std::atof(second_text.c_str()) << std::endl;
              if(std::atof(first_text.c_str()) < std::atof(second_text.c_str()))
                return -1;
              else if(std::atof(first_text.c_str()) > std::atof(second_text.c_str()))
                return 1;
              else
                return 0;
        }

        return 0;
      }
    }
  }
}
