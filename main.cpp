#include <iostream>
#include <vector>
#include <FL/Fl.H>
#include "MainWindow.h"
#include "User.h"

int main(int argc, char* argv[]) //Ensure main has these parameters
{
	//Set theming
	Fl::scheme("gtk+"); //Must be called before any other FLTK function

	//Initialize user
	std::shared_ptr<User> user = std::make_shared<User>();

	//Initialize window
	std::shared_ptr<MainWindow> win = std::make_shared<MainWindow>(user, 800, 800);

	//Display the window
	win->show(argc, argv); //For the first win.show function, pass argc and argv as parameters

	return Fl::run(); //Must be called at the end of main()

	/// NOTE TO SELF
	/// CHARACTER LIMIT IS 125 CHARACTERS, ENSURE CHAT IS LIMITED TO 125
	/// USERNAME 10 CHAR, CHAT MESSAGE 115
}