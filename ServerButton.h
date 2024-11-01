#pragma once
#include <FL/Fl_Button.H>

struct ServerButton
{
    ServerButton(int _x, int _y, int _w, int _h);
    void AddCallback(ServerButton *ref);
    static void StaticButtonListener(Fl_Widget* w, void* _userdata);
    void ButtonListener();

    Fl_Button mButton;
};