#pragma once

#include <FL/Fl.H>
#include <FL/Fl_Widget.H>
#include <FL/Fl_Output.H>
#include <FL/Fl_Window.H>
#include <FL/fl_draw.H>

#include <string>

/**
  Custom Fl_Output Class made in order to acommodate text centering.

  Assumes the same form as Fl_Output, except that the text will always be centered. Does not interact with user inputs, 
  and will ignores actions such as clicking, dragging, copying, etc.
*/
class Fl_Centered_Output : public Fl_Output
{
public:
	Fl_Centered_Output(int x, int y, int w, int h, const char* label = nullptr) : Fl_Output(x, y, w, h, label) {}

	void draw() override;
	int handle(int event) override;
private:
	int mSelectStart{ -1 };
	int mSelectEnd{ -1 };
};

