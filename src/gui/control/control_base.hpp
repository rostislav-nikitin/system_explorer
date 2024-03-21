#ifndef CONTROL_BASE_HPP
#define CONTROL_BASE_HPP

#include <wx/wx.h>

// RESPONSIBILITIES:
//  1. Show process data in a treelist
//  2. Generate EVT_SEARCH events when user typing text in a search box
//  3. Generate EVT_CONTEXT_MENU when user right-click on treelist
//  All the Alt-Del, Ctrl-Del evets will be processed by the AccelTable on the parent view

namespace SystemExplorer
{
    namespace Gui
    {
        namespace Control
        {
            class ControlBase : public wxPanel
            {
            protected:
                virtual void CreateChildControls();
                virtual void BindEvents();
                void Initialize();

            public:
                ControlBase(wxWindow *parent, wxWindowID Id = wxID_ANY);
            };
        }
    }
}

#endif