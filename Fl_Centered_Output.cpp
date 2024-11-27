#include "Fl_Centered_Output.h"

void Fl_Centered_Output::draw()
{
	//Draw widget background
	fl_color(Fl_Output::color());
	fl_rectf(x(), y(), w(), h());

	//Grab text stored within widget
	const char* text = value();
	//If text is present inside widget
	if (text && *text)
	{ 
		//Grab text font and size
		fl_font(Fl_Output::textfont(), Fl_Output::textsize());
		//Set text color
		fl_color(Fl_Output::textcolor());

		//Find dimensions of text
		int textW = fl_width(text);
		int textH = fl_height();

		//Calculate how much space should be left on the left side of the text
		int textX = x() + ((w() - textW) / 2);
		//Calculate the text baseline
		int textY = y() + ((h() + (textH / 2)) / 2);

		//Draw text
		fl_draw(text, textX, textY);
	}
}

int Fl_Centered_Output::handle(int event)
{
	return 0;
}
