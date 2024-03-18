#include "about_dialog_view_controller.hpp"

namespace SystemExplorer
{
    namespace Gui
    {
        namespace ViewController
        {
            AboutDialogViewController::AboutDialogViewController(wxWindow *parent, wxWindowID id)
                : wxDialog(parent, id, "About", wxDefaultPosition, wxSize(520, 480))
            {
                CreateChildControls();
            }

            void AboutDialogViewController::CreateChildControls()
            {
                wxBitmap *image = new wxBitmap("./resources/nut-64x64.png");
                wxBitmapBundle *bundle = new wxBitmapBundle(*image);
                wxStaticBitmap *icon = new wxStaticBitmap(this, wxID_ANY, *bundle);

                //icon->SetBitmap(*bundle);
                wxBoxSizer *mainSizer = new wxBoxSizer(wxVERTICAL);
                wxBoxSizer *bodySizer = new wxBoxSizer(wxHORIZONTAL);

                wxBoxSizer *textSizer = new wxBoxSizer(wxVERTICAL);
                textSizer->Add(new wxStaticText(this, wxID_ANY, "\tSystem Explorer is a powertool dedicated to speed up you OS related interactions."), 2, wxALIGN_LEFT | wxALL, 8);
                textSizer->Add(new wxStaticText(this, wxID_ANY, "LICENSE\nYou are free to:\n\
\tShare - copy and redistribute the material in any medium or format\n\
\tAdapt - remix, transform, and build upon the material\n\
Under the following terms:\n\
\tAttribution - You must give appropriate credit , provide a link to the license, and indicate if changes were made .\n\
\tNonCommercial - You may not use the material for commercial purposes."), 8, wxALL | wxALIGN_LEFT, 8);


                textSizer->Add(new wxStaticText(this, wxID_ANY, "Author: Rostyslav Nikitin"), 1, wxALIGN_RIGHT | wxALL, 8);
                textSizer->Add(new wxStaticText(this, wxID_ANY, "E-mail: rostislav.nikitin@gmail.com"), 1, wxALIGN_RIGHT | wxALL, 8);

                wxSizer *buttonSizer = this->CreateSeparatedButtonSizer(wxCLOSE);

                bodySizer->Add(icon, 0, wxALL, 8);
                bodySizer->Add(textSizer, 1, wxALL, 8);

                mainSizer->Add(bodySizer, 1, wxEXPAND | wxALL, 8);
                mainSizer->Add(buttonSizer, 0, wxEXPAND | wxALL, 8);
                this->SetSizer(mainSizer);
                //this->CreateButtonSizer(wxCLOSE);
            }
            
        }
    }
}