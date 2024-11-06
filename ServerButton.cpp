#include "ServerButton.h"
#include "Menu.h"

ServerButton::ServerButton(ServerButton* _ref, Menu* _parent, int _x, int _y, int _w, int _h) : Fl_Button(_x, _y, _w, _h, "HIDE")
{
    parent = _parent;
    callback(StaticButtonListener, (void*)_ref);
}

void ServerButton::StaticButtonListener(Fl_Widget* w, void* _userdata)
{
    //Once called, typecast the reference back to the button and call it's individual function
    ServerButton* buttonFunction = (ServerButton*)_userdata;
    buttonFunction->ButtonListener();
}

void ServerButton::ButtonListener()
{
    printf("Hey!\n");
    parent->ParentInitHost();
    parent->ChangeParentState(1);
}