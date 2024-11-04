#pragma once
class Timer
{
public:
	Timer(float _duration);

private:
	static void UpdateStatic(void* _userdata);
	virtual void Update();

	float mDuration;
};

