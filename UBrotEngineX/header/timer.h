///////////////////////////////////////////////////////////////////////////////////////////////////
// Filename: timer.h
///////////////////////////////////////////////////////////////////////////////////////////////////
#ifndef _TIMER_H_
#define _TIMER_H_


//////////////
// INCLUDES //
//////////////
#include <windows.h>


///////////////////////////////////////////////////////////////////////////////////////////////////
// Class name: Timer
/// The timer class serves to accurately measure the time it took to complete a frame. This is
/// necessary to synchronize objects that need a time component for functionalities like movement.
/// Example:
///		A timer can be used to calculate how much of a second has passed,
///		which can then be used to to translate the object accordingly.
///////////////////////////////////////////////////////////////////////////////////////////////////
class Timer
{
public:
	/**
	 * A constructor where all the member variables are initialised to 0.0f.
	 */
	Timer();
	/**
	 * Empty copy contructor
	 */
	Timer(const Timer&);
	~Timer();

	/**
	 * Initiaizes a high frequency timer, the starting time and the number of ticks per
	 * millisecond. Afterwards, the frame time is calculated.
	 */
	bool Initialize();
	/**
	 * Calculates the frame time and should therefore be called every frame. The frame time is
	 * simply the difference between the last call and this one. The resulting frame time is
	 * stored in \a m_frameTime and after its calculation, \a m_startTime is updated.
	 */
	void Frame();

	/**
	 * @return \a m_frameTime length of the last frame in milliseconds
	 */
	float GetTime();

private:
	INT64 m_frequency;
	float m_ticksPerMS;
	INT64 m_startTime;
	float m_frameTime;
};

#endif