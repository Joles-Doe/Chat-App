#pragma once
#include <FL/Fl_Window.H>
#include <FL/Fl_Flex.H>
#include <FL/Fl_Button.H>

struct ServerButton;
struct ClientButton;

class MainWindow : public Fl_Window
{
public:
    MainWindow();
    ~MainWindow();
private:
    Fl_Flex mMainBox;
    Fl_Button mServerButton;
    Fl_Button mClientButton;
};

struct ServerButton
{
    ServerButton(Fl_Window* _window, int _x, int _y, int _w, int _h);
    static void ButtonListener(Fl_Widget* w, void* _userdata);

    Fl_Button mButton;
};