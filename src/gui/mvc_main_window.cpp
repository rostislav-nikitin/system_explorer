#include "mvc_main_window.hpp"

MvcMainWindow::MvcMainWindow() : wxFrame(nullptr, wxID_ANY, "System Explorer", wxPoint(-1, -1), wxSize(800, 600))
{
    using SystemExplorer::Core::ProcessManager;
    using SystemExplorer::Gui::ViewController::ProcessTreeViewController;

    CreateStatusBar();

    wxBookCtrl *mainBook = new wxBookCtrl(this, wxID_ANY, wxPoint(10, 10), wxSize(800, 600));

    ProcessManager processManager;

    _processTreeViewController = new ProcessTreeViewController(
        mainBook, processManager, "Processes(Beta MVVP)", wxID_ANY, true);
    _processTreeViewController->Initialize();
}