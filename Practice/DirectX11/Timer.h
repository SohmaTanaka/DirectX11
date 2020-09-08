#pragma once

#include <chrono>

class Timer
{
public:
	Timer();
	double GetMilliSecondsElapsed();	//�o�ߎ���
	void Restart();	//�Ďn��
	bool Stop();	//��~
	bool Start();	//�J�n

private:
	bool isRunning = false;
#ifdef _WIN32
	std::chrono::time_point<std::chrono::steady_clock> start;
	std::chrono::time_point<std::chrono::steady_clock> stop;
#else
	std::chrono::time_point<std::chrono::system_clock> start;
	std::chrono::time_point<std::chrono::system_clock> stop;
#endif
};