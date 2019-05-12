#pragma once
#include <chrono>

class Timer
{
	using CLOCK = std::chrono::high_resolution_clock;
	CLOCK::time_point m_time;

public:
	Timer() { m_time = CLOCK::now(); }

	void reset() { m_time = CLOCK::now(); }
	float elapsed()
	{
		std::chrono::duration<float> time = CLOCK::now() - m_time;
		return time.count();
	}
};
