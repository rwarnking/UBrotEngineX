///////////////////////////////////////////////////////////////////////////////////////////////////
// Filename: timer.cpp
///////////////////////////////////////////////////////////////////////////////////////////////////
#include "../header/timer.h"


Timer::Timer()
{
	INT64 m_frequency = 0;
	float m_ticksPerMS = 0.0f;
	INT64 m_startTime = 0;
	float m_frameTime = 0.0f;
}


Timer::Timer(const Timer& other)
{
}


Timer::~Timer()
{
}


bool Timer::Initialize()
{
	// Check if the system support high frequency timers
	QueryPerformanceFrequency((LARGE_INTEGER*)&m_frequency);
	if (m_frequency == 0)
	{
		return false;
	}

	// Calculate the number of ticks per millisecond
	m_ticksPerMS = (float)(m_frequency / 1000);

	QueryPerformanceCounter((LARGE_INTEGER*)&m_startTime);

	return true;
}


void Timer::Frame()
{
	INT64 currentTime;
	float timeDifference;


	// Check the current time and then calculate the difference
	QueryPerformanceCounter((LARGE_INTEGER*)&currentTime);

	timeDifference = (float)(currentTime - m_startTime);

	m_frameTime = timeDifference / m_ticksPerMS;

	m_startTime = currentTime;

	return;
}


float Timer::GetTime()
{
	return m_frameTime;
}