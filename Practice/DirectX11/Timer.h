#pragma once

#include <chrono>

class Timer
{
public:
	Timer();
	double GetMilliSecondsElapsed();	//経過時間
	void Restart();	//再始動
	bool Stop();	//停止
	bool Start();	//開始

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