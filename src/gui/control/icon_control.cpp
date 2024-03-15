
#include "icon_control.hpp"

namespace SystemExplorer
{
    namespace Gui
    {
        namespace Control
        {
            IconControl::IconControl(wxWindow *parent, wxWindowID id, 
                wxString fileName, wxString labelText, wxPoint position, wxSize size) 
                    : wxPanel(parent, id, position, size)
                {
                    int ICON_WIDTH = 16 + 2 + 2;
                    wxPoint labelPos = wxPoint(position.x + ICON_WIDTH, position.y);

                    wxRect parentRect = parent->GetRect();
                    wxSize labelSize = wxSize(size.GetWidth() - ICON_WIDTH, parentRect.GetHeight());

                    _label = new wxStaticText(this, wxID_ANY, labelText, labelPos, labelSize);

                    wxVector<wxBitmap> bitmaps;
                    bitmaps.push_back(wxBitmap(fileName));

                    _icon = new wxStaticBitmap(this, wxID_ANY, wxBitmapBundle::FromBitmaps(bitmaps));

                    _bsSizer = new wxBoxSizer(wxHORIZONTAL);
                    _bsSizer->Add(_icon, 0, wxALL | wxEXPAND, 2);
                    _bsSizer->Add(_label, 1, wxALL | wxEXPAND, 2);

                    this->SetSizer(_bsSizer);
                }

            wxString IconControl::GetText() const
            {
                return _label->GetLabelText();
            }
            void IconControl::SetText(wxString text)
            {
                return _label->SetLabelText(text);
            }
        }
    }
}