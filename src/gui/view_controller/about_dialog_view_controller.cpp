#include "about_dialog_view_controller.hpp"

namespace SystemExplorer
{
    namespace Gui
    {
        namespace ViewController
        {
            AboutDialogViewController::AboutDialogViewController(wxWindow *parent, wxWindowID id)
                : wxDialog(parent, id, "About", wxDefaultPosition, wxSize(540, 540))
            {
                CreateChildControls();
            }

            void AboutDialogViewController::CreateChildControls()
            {
                // Main logo
                wxBitmap *imageLogo = new wxBitmap("./resources/nut-64x64.png");
                wxBitmapBundle *bundleLogo = new wxBitmapBundle(*imageLogo);
                wxStaticBitmap *iconLogo = new wxStaticBitmap(this, wxID_ANY, *bundleLogo);

                // wxWidgets logo
                wxBitmap *imageWxLogo = new wxBitmap("./resources/wxwidgets_logo-64x48.png");
                wxBitmapBundle *bundleWxLogo = new wxBitmapBundle(*imageWxLogo);
                wxStaticBitmap *iconWxLogo = new wxStaticBitmap(this, wxID_ANY, *bundleWxLogo);

                wxBoxSizer *mainSizer = new wxBoxSizer(wxVERTICAL);

                // Header
                wxBoxSizer *headerSizer = new wxBoxSizer(wxHORIZONTAL);
                wxBoxSizer *headerLeftSizer = new wxBoxSizer(wxVERTICAL);
                headerLeftSizer->Add(new wxStaticText(this, wxID_ANY, "\tSystem Explorer is a powertool dedicated to speed up \nyour OS related interactions."), 0, wxALIGN_LEFT | wxALL, 8);

                wxStaticText *stLicense = new wxStaticText(this, wxID_ANY, "LICENSE\nYou are free to:\n\
\tShare - copy and redistribute the material in any medium or format\n\
\tAdapt - remix, transform, and build upon the material\n\
Under the following terms:\n\
\tAttribution - You must give appropriate credit , provide a link to the license, and indicate if changes were made .\n\
\tNonCommercial - You may not use the material for commercial purposes.");
                wxSize szLicense = stLicense->GetSize();
                szLicense.SetHeight(190);
                stLicense->SetMinSize(szLicense);
                headerLeftSizer->Add(stLicense, 0, wxALL | wxALIGN_LEFT, 8);

                headerSizer->Add(iconLogo, 0, wxALL, 8);
                headerSizer->Add(headerLeftSizer, 1, wxEXPAND | wxALL, 8);


                // Footer
                wxBoxSizer *footerSizer = new wxBoxSizer(wxHORIZONTAL);
                wxBoxSizer *footerRightSizer = new wxBoxSizer(wxVERTICAL);

                footerRightSizer->Add(new wxStaticText(this, wxID_ANY, "Powered By: wxWidgets"), 0, wxALIGN_RIGHT | wxALL, 8);
                footerRightSizer->Add(new wxStaticText(this, wxID_ANY, "Author: Rostyslav Nikitin"), 0, wxALIGN_RIGHT | wxALL, 8);
                footerRightSizer->Add(new wxStaticText(this, wxID_ANY, "E-mail: rostislav.nikitin@gmail.com"), 0, wxALIGN_RIGHT | wxALL, 8);

                footerSizer->Add(iconWxLogo, 0, wxALL, 8);
                footerSizer->Add(footerRightSizer, 1, wxEXPAND | wxALL, 8);

                wxSizer *buttonSizer = this->CreateSeparatedButtonSizer(wxCLOSE);

                mainSizer->Add(headerSizer, 0, wxEXPAND | wxALL, 8);
                mainSizer->Add(footerSizer, 0, wxEXPAND | wxALL, 8);
                mainSizer->Add(buttonSizer, 0, wxEXPAND | wxALL, 8);
                this->SetSizer(mainSizer);
                //this->CreateButtonSizer(wxCLOSE);
            }
            
        }
    }
}