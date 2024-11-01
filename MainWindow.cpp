#include "MainWindow.h"

MainWindow::MainWindow()
    : Fl_Window(800, 800, "TEST"),
    mMainBox(0, 0, 0, 0),
    mServerButton(100, 100, 200, 200),
    mClientButton(400, 100, 200, 200),
    mTestButton(0, 0, 0, 0)
{

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