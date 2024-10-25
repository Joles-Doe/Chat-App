#pragma once
#include <FL/FL.h>
#include <FL/FL_Window.h>
#include <FL/Fl_Flex.h>
#include <FL/Fl_Menu_Bar.h>
#include <FL/Fl_Box.h>
#include <FL/Fl_Button.h>

class MainWindow : public Fl_Window
{
private:
	//Content is initialized from top to bottom
	Fl_Flex mainCol;
	Fl_Menu_Bar menuBar;
	Fl_Box content;
public:
	MainWindow();
};

