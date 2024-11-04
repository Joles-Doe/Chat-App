#pragma once
#include "Timer.h"
#include <FL/Fl_Window.H>
#include <FL/Fl_Button.H>
//#include <FL/Fl_Text_Buffer.H>
//#include <FL/Fl_Text_Display.H>
#include <FL/Fl_Group.H>

#include "User.h"
#include "Menu.h"

#include <memory>

class MainWindow : public Fl_Window, Timer
{
public:
    MainWindow(std::shared_ptr<User> _user, int _w, int _h);
    ~MainWindow();

    void Update();

    std::shared_ptr<User> GetUser();
    void SetUser(std::shared_ptr<User> _user);

private:
    std::shared_ptr<User> user;

    Menu menu;

};