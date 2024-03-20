#include "about_dialog_view_controller.hpp"

namespace SystemExplorer
{
    namespace Gui
    {
        namespace ViewController
        {
            AboutDialogViewController::AboutDialogViewController(wxWindow *parent, wxWindowID id)
                : wxDialog(parent, id, "About", wxDefaultPosition, wxSize(600, 540))
            {
                CreateChildControls();
            }

            void AboutDialogViewController::CreateChildControls()
            {
                // Main logo
                wxBitmapBundle *bundleLogo = new wxBitmapBundle(*bin2c_nut_64x64_png);
                wxStaticBitmap *iconLogo = new wxStaticBitmap(this, wxID_ANY, *bundleLogo);

                // wxWidgets logo
                wxBitmapBundle *bundleWxLogo = new wxBitmapBundle(*bin2c_wxwidgets_logo_64x48_png);
                wxStaticBitmap *iconWxLogo = new wxStaticBitmap(this, wxID_ANY, *bundleWxLogo);

                // UA logo
                wxBitmapBundle *bundleUaLogo = new wxBitmapBundle(*bin2c_ua_64x43_png);
                wxStaticBitmap *iconUaLogo = new wxStaticBitmap(this, wxID_ANY, *bundleUaLogo);

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
                szLicense.SetHeight(150);
                stLicense->SetMinSize(szLicense);
                headerLeftSizer->Add(stLicense, 0, wxALL | wxALIGN_LEFT, 8);

                headerSizer->Add(iconLogo, 0, wxALL, 8);
                headerSizer->Add(headerLeftSizer, 1, wxEXPAND | wxALL, 8);


                // Footer
                wxBoxSizer *poweredSizer = new wxBoxSizer(wxHORIZONTAL);
                wxBoxSizer *poweredRightSizer = new wxBoxSizer(wxVERTICAL);

                poweredRightSizer->Add(new wxStaticText(this, wxID_ANY, "Powered By: wxWidgets"), 0, wxALIGN_RIGHT | wxALL, 8);

                poweredSizer->Add(iconWxLogo, 0, wxALL, 8);
                poweredSizer->Add(poweredRightSizer, 1, wxEXPAND | wxALL, 8);

                // Author
                wxBoxSizer *authorSizer = new wxBoxSizer(wxHORIZONTAL);
                wxBoxSizer *authroRightSizer = new wxBoxSizer(wxVERTICAL);

                authroRightSizer->Add(new wxStaticText(this, wxID_ANY, "Author: Rostyslav Nikitin"), 0, wxALIGN_RIGHT | wxALL, 8);
                authroRightSizer->Add(new wxStaticText(this, wxID_ANY, "E-mail: rostislav.nikitin@gmail.com"), 0, wxALIGN_RIGHT | wxALL, 8);

                authorSizer->Add(iconUaLogo, 0, wxALL, 8);
                authorSizer->Add(authroRightSizer, 1, wxEXPAND | wxALL, 8);

                wxSizer *buttonSizer = this->CreateSeparatedButtonSizer(wxCLOSE);

                mainSizer->Add(headerSizer, 0, wxEXPAND | wxALL, 8);
                mainSizer->Add(poweredSizer, 0, wxEXPAND | wxALL, 8);
                mainSizer->Add(authorSizer, 0, wxEXPAND | wxALL, 8);
                mainSizer->Add(buttonSizer, 0, wxEXPAND | wxALL, 8);
                this->SetSizer(mainSizer);
                //this->CreateButtonSizer(wxCLOSE);
            }
            
        }
    }
}