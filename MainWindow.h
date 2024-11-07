#pragma once
#include "Timer.h"
#include <FL/Fl_Window.H>
#include <FL/Fl_Button.H>
#include <FL/Fl_Group.H>

#include "User.h"
#include "Menu.h"
#include "InChat.h"

#include <memory>
#include <vector>

class MainWindow : public Fl_Window, Timer
{
public:
    MainWindow(std::shared_ptr<User> _user, int _w, int _h);
    ~MainWindow();

    void Update();

    std::shared_ptr<User> GetUser();
    void SetUser(std::shared_ptr<User> _user);

    void ChangeState(int _state);

    void InitHost();
    void InitClient();

private:
    std::shared_ptr<User> user;

    bool mStateChanged{ false };
    int mState{ 0 };

    Menu menu;
    InChat chat;
};