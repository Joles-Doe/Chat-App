#include "Fl_Centered_Output.h"

void Fl_Centered_Output::draw()
{
	//Draw widget background
	fl_color(FL_WHITE);
	fl_rectf(x(), y(), w(), h());

	//Grab text stored within widget
	const char* text = value();
	//If text is present inside widget
	if (text && *text)
	{ 
		//Grab text font and size
		fl_font(textfont(), textsize());
		//Set text color
		fl_color(textcolor());

		//Find dimensions of text
		int textW = fl_width(text);
		int textH = fl_height();

		//Calculate how much space should be left on the left side of the text
		int textX = x() + ((w() - textW) / 2);
		//Calculate the text baseline
		int textY = y() + ((h() + (textH / 2)) / 2);

		if ((mSelectStart != -1) && (mSelectEnd != -1) && (mSelectStart != mSelectEnd))
		{
			Fl_Color(FL_BLUE);
			
			std::string selectionText = text;

			int startX = x() + fl_width(selectionText.substr(0, mSelectStart).c_str());
			int endX = x() + fl_width(selectionText.substr(mSelectStart, mSelectEnd).c_str());

			fl_rectf(startX, y(), endX - startX, h());
		}


		//Draw text
		fl_draw(text, textX, textY);
	}
}
