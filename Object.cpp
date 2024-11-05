#include "Object.h"

Object::Object(void* _widget) : mWidgetReference(_widget)
{
}

void* Object::GetWidgetReference()
{
	return mWidgetReference;
}

bool Object::GetActive()
{
	return mActive;
}

void Object::SetActive(bool _active)
{
	mActive = _active;
	Fl_Widget* widget = (Fl_Widget*)mWidgetReference;

	if (_active == false)
	{
		if (widget->visible() == true)
		{
			widget->hide();
		}
	}
	else
	{	
		if (widget->visible() == false)
		{
			widget->show();
		}
	}
}

void Object::Update()
{

}
