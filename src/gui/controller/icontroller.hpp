#ifndef ICONTROLLER_HPP
#define ICONTROLLER_HPP

class IController
{
protected:
    void CreateChildControls();
    void BindEvents();
    void BindDate();
public:
    void Run();
};

#endif