#include "MainWindow.h"

MainWindow::MainWindow()
    : Fl_Window(800, 800, "TEST"),
    mMainBox(0, 0, 0, 0),
    mServerButton(100, 100, 200, 200),
    mClientButton(400, 100, 200, 200)
{

}

MainWindow::~MainWindow()
{
    //Fl::remove_timeout(on_tick, this);
}

ServerButton::ServerButton(Fl_Window* _window, int _x, int _y, int _w, int _h) : mButton(_x, _y, _w, _h)
{
    mButton.callback(ButtonListener, 0);
}

void ServerButton::ButtonListener(Fl_Widget *_widget, void *_userdata)
{

}
