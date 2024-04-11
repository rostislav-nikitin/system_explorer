#ifndef LIBUI_CONTROL_ICON_CONTROL_HPP
#define LIBUI_CONTROL_ICON_CONTROL_HPP

#include <wx/wx.h>

namespace UI
{
    namespace Control
    {
        // The class that represets a control with a labeled icon.
        class IconControl : public wxPanel
        {
        private:
            wxStaticText    *_label;
            wxStaticBitmap  *_icon;
            wxBoxSizer      *_bsSizer;

        public:
            // The constructor.
            IconControl(
                wxWindow *parent, 
                wxWindowID id,
                wxBitmap icon, 
                wxString labelText,
                wxPoint position = wxDefaultPosition, 
                wxSize size = wxDefaultSize);

            // The Text property getter. It gets a label text.
            wxString GetText() const;
            // The Text property setter. It sets a label text.
            void SetText(wxString text);
        };
    }
}

#endif