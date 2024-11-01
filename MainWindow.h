#pragma once
#include <FL/Fl_Window.H>
#include <FL/Fl_Flex.H>
#include <FL/Fl_Button.H>
#include "ServerButton.h"
#include "User.h"
#include <memory>

struct ClientButton;

class MainWindow : public Fl_Window
{
public:
    MainWindow();
    ~MainWindow();

    std::shared_ptr<User> GetUser();
    void SetUser(std::shared_ptr<User> _user);
private:
    std::shared_ptr<User> user;

    Fl_Flex mMainBox;
    Fl_Button mServerButton;
    Fl_Button mClientButton;

    ServerButton mTestButton;
};

