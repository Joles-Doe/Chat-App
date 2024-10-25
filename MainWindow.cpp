#include "MainWindow.h"
#include <stdexcept>
#include <iostream>

MainWindow::MainWindow() 
	: Fl_Window(700, 540, "TEST"),
	mainCol(0, 0, w(), h()),
	menuBar(0, 0, 0, 0),
	content(0, 0, 0, 0)
{
	menuBar.color(fl_rgb_color(255, 10, 63));

	content.box(FL_ENGRAVED_BOX);
	content.color(fl_rgb_color(255, 10, 100));


	//Add the flexbox to the window
	add_resizable(mainCol);
	//Add the rest of the elements to the flexbox
	mainCol.add_resizable(menuBar);
	mainCol.add_resizable(content);
	//Fix the menubar height to 70px
	mainCol.fixed(menuBar, 70);
}
