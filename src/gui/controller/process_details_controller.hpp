#ifndef PROCESS_DETAILS_CONTROLLER_HPP
#define PROCESS_DETAILS_CONTROLLER_HPP

#include <wx/wx.h>
#include <wx/bookctrl.h>

class ProcessDetailsController
{
    wxBookCtrl *_mainBook;
public:
    ProcessDetailsController(wxBookCtrl *mainBook);
};

#endif