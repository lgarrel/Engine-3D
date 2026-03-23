#include "pch.h"
#include "Timer.h"

Timer::Timer() : m_secondsPerCount(0.0), m_deltaTime(-1.0), m_baseTime(0), m_currTime(0),
				 m_prevTime(0), m_pausedTime(0), m_stopTime(0), m_isStopped(false)
{
	int64_t countPerSecond;
	QueryPerformanceFrequency((LARGE_INTEGER*)&countPerSecond);
	m_secondsPerCount = 1.0 / (double)countPerSecond;
}

float Timer::DeltaTime()
{
	return (float)m_deltaTime;
}

float Timer::TotalTime()
{
	float totalTime = 0.f;

	if(m_isStopped)
	{
		totalTime = (float)((m_stopTime - m_pausedTime) - m_baseTime) * m_secondsPerCount;
		return totalTime;
	}
	else
	{
		totalTime = (float)((m_currTime - m_pausedTime) - m_baseTime) * m_secondsPerCount;
		return totalTime;
	}

}

void Timer::Start()
{
	int64_t startTime;
	QueryPerformanceCounter((LARGE_INTEGER*)&startTime);

	if(m_isStopped)
	{
		m_pausedTime += (startTime - m_stopTime);

		m_prevTime = startTime;

		m_stopTime = 0;
		m_isStopped = false;
	}
}

void Timer::Reset()
{
	int64_t currTime;
	QueryPerformanceCounter((LARGE_INTEGER*)&currTime);

	m_prevTime = currTime;
	m_baseTime = currTime;
	m_stopTime = 0;
	m_isStopped = false;
}

void Timer::Stop()
{
	if (m_isStopped == true)
		return;

	int64_t currTime;
	QueryPerformanceCounter((LARGE_INTEGER*)&currTime);

	m_stopTime = currTime;
	m_isStopped = true;
}

void Timer::Tick()
{
	if(m_isStopped)
	{
		m_deltaTime = 0.0;
		return;
	}

	int64_t currTime;
	QueryPerformanceCounter((LARGE_INTEGER*)&currTime);
	m_currTime = currTime;

	// Time difference between this frame and the previous
	m_deltaTime = (m_currTime - m_prevTime) * m_secondsPerCount;

	m_prevTime = m_currTime;

	if(m_deltaTime < 0.0)
	{
		m_deltaTime = 0.0;
	}

}
