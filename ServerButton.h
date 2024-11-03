#pragma once
#include <FL/Fl_Button.H>

class Menu;

class ServerButton : public Fl_Button
{
public:
    ServerButton(ServerButton *_ref, Menu *_parent, int _x, int _y, int _w, int _h);

private:
    Menu *parent;

    static void StaticButtonListener(Fl_Widget* w, void* _userdata);
    void ButtonListener();
};