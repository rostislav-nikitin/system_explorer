#ifndef PROCESS_DETAILS_VIEW_CONTROLLER_HPP
#define PROCESS_DETAILS_VIEW_CONTROLLER_HPP

#include <wx/wx.h>
#include <wx/bookctrl.h>

class ProcessDetailsViewController
{
    wxBookCtrl *_mainBook;
public:
    ProcessDetailsViewController(wxBookCtrl *mainBook);
};

#endif