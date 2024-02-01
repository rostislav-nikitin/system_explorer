#ifndef PROCESSES_TREE_LIST_ITEM_COMPARATOR
#define PROCESSES_TREE_LIST_ITEM_COMPARATOR

#include <wx/treelist.h>

namespace SystemExplorer
{
  namespace Gui
  {
    class ProcessesTreeListItemComparator : public wxTreeListItemComparator
    {
    public:
      virtual int Compare (wxTreeListCtrl *treelist, unsigned column, wxTreeListItem first, wxTreeListItem second) override;
    };
  }
}

#endif
