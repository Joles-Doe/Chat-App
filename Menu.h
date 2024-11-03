#pragma once

#include <FL/Fl.H>
#include <FL/Fl_Group.H>

#include <memory>
#include "User.h"


#include "ServerButton.h"

class Menu : public Fl_Group 
{
public:
	Menu(std::shared_ptr<User> _user, int _x, int _y, int _w, int _h);

private:
	std::shared_ptr<User> user;

	ServerButton mServerButton;
};