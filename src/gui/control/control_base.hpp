#ifndef CONTROL_BASE_HPP
#define CONTROL_BASE_HPP

#include <wx/wx.h>

/** 
 * @file
 * @brief The base class for the all custom controls. 
*/

namespace SystemExplorer
{
    namespace Gui
    {
        namespace Control
        {
            /**
                The base class for the all custom controls. 
            */
            class ControlBase : public wxPanel
            {
            protected:
                /** 
                 * The template method creates a child controls.
                 * It should be overriden in the derived classes.
                 */
                virtual void CreateChildControls();
                /** 
                 * The template method that binds events to the thia and child controls.
                 * It should be overriden in the derived classes.
                 */
                virtual void BindEvents();
                /** 
                 * The method that do all the iniitialization work, calls CreateChildControls, BindEvents.
                 * It should be called in the top derived class inside a constructor.
                 */
                void Initialize();

            public:
                /** 
                 * The constructor.
                 */ 
                ControlBase(wxWindow *parent, wxWindowID Id = wxID_ANY);
            };
        }
    }
}

#endif