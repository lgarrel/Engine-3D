#pragma once
class Timer
{
	double m_secondsPerCount;
	double m_deltaTime;

	int64_t m_prevTime;
	int64_t m_currTime;
	int64_t m_pausedTime;
	int64_t m_baseTime;
	int64_t m_stopTime;

	bool m_isStopped;

public:

	Timer();

	float DeltaTime();
	float TotalTime();

	void Start();
	void Reset();
	void Stop();
	void Tick();

};

