#pragma once

/*
  Abstract class used to call FLTK's repeat function, allowing a function to be called once every tick.
*/
class Timer
{
public:
	Timer(float _duration);

  private:
	//Static reference to Update() function
	static void UpdateStatic(void* _userdata);
	//Virtual update function, allows derived class to have their own function that repeats every tick
	virtual void Update();

	float mDuration;
};

