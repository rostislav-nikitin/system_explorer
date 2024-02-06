#ifndef PROCESS_CONTROLLER_HPP
#define PROCESS_CONTROLLER_HPP

#include <wx/wx.h>

class MainWindow;

class ProcessController
{
    wxPanel *_processesTab;

    void CreateChildControls();
    void BindEvents();
    void BindData();

public:
    ProcessController(wxPanel *processesTab);

};

#endif