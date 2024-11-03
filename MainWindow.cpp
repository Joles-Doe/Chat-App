#include "MainWindow.h"

MainWindow::MainWindow(std::shared_ptr<User> _user, int _w, int _h)
    : Fl_Window(_w, _h, "TEST"),
    menu(_user, 0, 0, _w, _h)
{
    user = _user;

    menu.show();
}

MainWindow::~MainWindow()
{
    
}

std::shared_ptr<User> MainWindow::GetUser()
{
    return user;
}

void MainWindow::SetUser(std::shared_ptr<User> _user)
{
    user = _user;
}