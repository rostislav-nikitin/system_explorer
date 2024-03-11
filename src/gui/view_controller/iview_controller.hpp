#ifndef IVIEWCONTROLLER_HPP
#define IVIEWCONTROLLER_HPP

namespace SystemExplorer
{
    namespace Gui
    {
        namespace ViewController
        {
            class IViewController
            {
            protected:
                virtual void CreateChildControls() = 0;
                virtual void BindEvents() = 0;
                virtual void BindData_Before() = 0;
                virtual void BindData() = 0;
            public:
                virtual ~IViewController() { }
            };
        }
    }
}

#endif