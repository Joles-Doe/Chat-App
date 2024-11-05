#pragma once

#include "Object.h"
#include <FL/Fl.H>
#include <FL/Fl_Window.H>

#include <memory>
#include "User.h"

#include "ServerButton.h"

class Menu : public Fl_Window, Object 
{
public:
	Menu(std::shared_ptr<User> _user, int _x, int _y, int _w, int _h);

	void Update();

private:
	std::shared_ptr<User> user;

	ServerButton mServerButton;
};