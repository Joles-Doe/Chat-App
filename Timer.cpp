#include "Timer.h"

#include <FL/Fl.H>

Timer::Timer(float _duration) : mDuration(_duration)
{
	Fl::add_timeout(_duration, UpdateStatic, this);
}

void Timer::UpdateStatic(void* _userdata)
{
	Timer* self = (Timer*)_userdata;
	Fl::repeat_timeout(self->mDuration, UpdateStatic, _userdata);
	self->Update();
}

void Timer::Update()
{
}