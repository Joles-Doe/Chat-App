#pragma once

#include <FL/Fl.H>
#include <FL/Fl_Widget.H>
#include <FL/Fl_Output.H>
#include <FL/Fl_Window.H>
#include <FL/fl_draw.H>

#include <string>

class Fl_Centered_Output : public Fl_Output
{
public:
	Fl_Centered_Output(int x, int y, int w, int h, const char* label = nullptr) : Fl_Output(x, y, w, h, label) {}

	void draw() override;

private:
	int mSelectStart{ -1 };
	int mSelectEnd{ -1 };
};

