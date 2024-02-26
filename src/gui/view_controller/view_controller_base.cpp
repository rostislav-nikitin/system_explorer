#include "view_controller_base.hpp"

#include <iostream>

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
                            std::cout << "CREATE CHILD CONTROLS" << std::endl;
            }
            void ViewControllerBase::BindEvents()
            {

            }
            void ViewControllerBase::BindData()
            {

            }
        }
    }
}