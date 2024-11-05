#pragma once

#include <FL/Fl_Widget.H>

class Object
{
private:
	void* mWidgetReference;

	bool mActive{ true };

public:
	Object(void* _widget);

	void* GetWidgetReference();

	bool GetActive();
	void SetActive(bool _active);

	virtual void Update();
};

