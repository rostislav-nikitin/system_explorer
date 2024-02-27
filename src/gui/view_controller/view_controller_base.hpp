#ifndef VIEW_CONTROLLER_BASE
#define VIEW_CONTROLLER_BASE

#include <string>

#include <wx/wx.h>
#include <wx/bookctrl.h>

#include "iview_controller.hpp"

namespace SystemExplorer
{
    namespace Gui
    {
        namespace ViewController
        {
            class ViewControllerBase : public IViewController, public wxPanel
            {
            protected:
                wxBookCtrl *_book;

                ViewControllerBase(wxBookCtrl *book, std::string title, wxWindowID id = wxID_ANY, bool useByDefault = false);

                virtual void CreateChildControls() override;
                virtual void BindEvents() override;
                virtual void BindData() override;

                virtual void PostInitialize();

                
                void SetHelpStatusText(wxString const &text);
                wxStatusBar *GetStatusBar() const;
                wxFrame *GetTopFrame() const;

            public:
                virtual void Initialize();
            };
        }
    }
    
} // namespace SystemExplorer

#endif