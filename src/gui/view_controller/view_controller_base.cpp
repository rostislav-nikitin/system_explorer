#include "view_controller_base.hpp"

namespace SystemExplorer
{
    namespace Gui
    {
        namespace ViewController
        {
            ViewControllerBase::ViewControllerBase(wxBookCtrl *book, std::string title, 
                wxWindowID id, bool useByDefault) : wxPanel(book, id), _book(book)
            {
                _book->AddPage(this, title, useByDefault);
            }

            void ViewControllerBase::Initialize()
            {
                CreateChildControls();
                BindEvents();
                BindData();
                PostInitialize();
            }

            void ViewControllerBase::PostInitialize()
            {
            }

            void ViewControllerBase::CreateChildControls()
            {
            }
            void ViewControllerBase::BindEvents()
            {
            }
            void ViewControllerBase::BindData()
            {
            }

            void ViewControllerBase::SetHelpStatusText(wxString const &text)
            {
                wxStatusBar *statusBar = GetStatusBar();

                if(statusBar != nullptr)
                {
                    statusBar->SetStatusText(text);
                }
            }
            wxStatusBar *ViewControllerBase::GetStatusBar() const
            {
                wxFrame *frame = GetTopFrame();

                if(frame == nullptr)
                    return nullptr;

                return frame->GetStatusBar();
            }

            wxFrame *ViewControllerBase::GetTopFrame() const
            {
                wxWindow *window = _book;
                while(window != nullptr && window->GetParent() != nullptr)
                {
                    window = window->GetParent();
                }

                wxFrame *frame = dynamic_cast<wxFrame *>(window);

                return frame;

            }

        }
    }
}