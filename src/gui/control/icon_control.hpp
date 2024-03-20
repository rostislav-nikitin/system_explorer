#ifndef ICON_CONTROL_HPP
#define ICON_CONTROL_HPP

#include <wx/wx.h>

namespace SystemExplorer
{
    namespace Gui
    {
        namespace Control
        {
            class IconControl : public wxPanel
            {
            private:
                wxStaticText    *_label;
                wxStaticBitmap  *_icon;
                wxBoxSizer      *_bsSizer;

            public:
                IconControl(
                    wxWindow *parent, 
                    wxWindowID id,
                    wxBitmap icon, 
                    wxString labelText,
                    wxPoint position = wxDefaultPosition, 
                    wxSize size = wxDefaultSize);

                wxString GetText() const;
                void SetText(wxString text);
            };
        }
    }
}

#endif