#include "MainWindow.h"

MainWindow::MainWindow(std::shared_ptr<User> _user, int _w, int _h)
    : Fl_Window(_w, _h, "TEST"), Timer(1.0f / 60.0f),
    menu(_user, this, 0, 0, _w, _h),
    chat(_user, 0, 0, _w, _h)
{
    MainWindow::size_range(500, 500);
    user = _user;

    menu.show();
}

MainWindow::~MainWindow()
{
    
}

//Update function - call each group's update function if they're active
void MainWindow::Update()
{
    switch (mState)
    {
    case 0:
        chat.hide();
        if (menu.visible() == false)
        {
            menu.show();
        }
        menu.Update();
        break;
    case 1:
        menu.hide();
        if (chat.visible() == false)
        {
            chat.show();
        }
        chat.Update();
        break;
    }
}

std::shared_ptr<User> MainWindow::GetUser()
{
    return user;
}

void MainWindow::SetUser(std::shared_ptr<User> _user)
{
    user = _user;
}

void MainWindow::ChangeState(int _state)
{
    mState = _state;
}
