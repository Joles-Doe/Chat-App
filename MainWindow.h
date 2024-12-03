#pragma once
#include "Timer.h"
#include <FL/Fl_Window.H>
#include <FL/Fl_PNG_Image.H>
#include <FL/Fl_Button.H>
#include <FL/Fl_Group.H>

#include "User.h"
#include "Menu.h"
#include "InChat.h"

#include <memory>
#include <vector>

/*
  Class used to encapsulate the FLTK logic.

  MainWindow represents the actual window, and hold the subsequent windows used to navigate menus.
  Additionally, MainWindow calls the Update() function present in user and the subsequent windows every tick
*/
class MainWindow : public Fl_Window, Timer
{
public:
    MainWindow(std::shared_ptr<User> _user, int _w, int _h);
    ~MainWindow();

    //Update function - automatically called once every tick.
    void Update();

    //Returns a pointer to the user
    std::shared_ptr<User> GetUser();
    //Sets the user pointer
    void SetUser(std::shared_ptr<User> _user);

    //Changes window state - hides and shows specific windows dependent on value
    /// <param name="0:"> Shows the Menu Window </param>
    /// <param name="1:"> Shows the Chat Window </param>
    void ChangeState(int _state);

private:
    std::shared_ptr<Fl_PNG_Image> mWindowIcon;

    std::shared_ptr<User> user;

    bool mStateChanged{ false };
    int mState{ 0 };

    std::string mChatVersion;

    Menu menu;
    InChat chat;
};