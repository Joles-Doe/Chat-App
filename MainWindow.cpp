#include "MainWindow.h"

MainWindow::MainWindow(std::shared_ptr<User> _user, int _w, int _h)
    : Fl_Window(_w, _h, "TEST"), Timer(1.0f / 60.0f),
    menu(_user, 0, 0, _w, _h)
{
    user = _user;

    menu.show();
}

MainWindow::~MainWindow()
{
    
}

//Update function - call each group's update function if they're active
void MainWindow::Update()
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