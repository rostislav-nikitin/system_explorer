#ifndef ICONTROLLER_HPP
#define ICONTROLLER_HPP

namespace SystemExplorer
{
    namespace Gui
    {
        namespace Controller
        {
            class IController
            {
            protected:
                virtual void CreateChildControls() = 0;
                virtual void BindEvents() = 0;
                virtual void BindData() = 0;
            public:
                virtual void Run() = 0;

                virtual ~IController() { }
            };
        }
    }
}

#endif