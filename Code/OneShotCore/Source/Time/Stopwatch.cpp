#include "CoreHeaders.h"
#include "Time/Stopwatch.h"

CStopwatch::CStopwatch() :
m_bIsRunning(false),
m_bIsPaused(false),
m_i64StartCount(0),
m_i64PauseStartCount(0),
m_i64CulmativePauseCount(0),
m_i64PreviousCount(0),
m_dSecondsPerCount(0.0)
{
}

CStopwatch::~CStopwatch()
{
}

void CStopwatch::Start()
{
	if(!m_bIsRunning)
	{
		__int64 i64CountsPerSecond = 0;
		QueryPerformanceFrequency((LARGE_INTEGER*)&i64CountsPerSecond);
		m_dSecondsPerCount = 1.0 / i64CountsPerSecond;

		QueryPerformanceCounter((LARGE_INTEGER*)&m_i64StartCount);
		m_i64PreviousCount = m_i64StartCount;
		m_bIsRunning = true;
	}
}

void CStopwatch::Pause()
{
	if(m_bIsRunning && !m_bIsPaused)
	{
		QueryPerformanceCounter((LARGE_INTEGER*)&m_i64PauseStartCount);
		m_i64PreviousCount = m_i64PauseStartCount;
		m_bIsPaused = true;
	}
}

void CStopwatch::Resume()
{
	if(m_bIsRunning && m_bIsPaused)
	{
		__int64 i64ResumeCount = 0;
		QueryPerformanceCounter((LARGE_INTEGER*)&i64ResumeCount);
		m_i64CulmativePauseCount += (i64ResumeCount - m_i64PauseStartCount);
		m_i64PauseStartCount = 0;
		m_i64PreviousCount = i64ResumeCount;
		m_bIsPaused = false;
	}
}

void CStopwatch::Reset()
{
	if(m_bIsRunning && !m_bIsPaused)
	{
		m_i64CulmativePauseCount = 0;
		m_i64PauseStartCount = 0;
		m_i64PreviousCount = m_i64StartCount;
		QueryPerformanceCounter((LARGE_INTEGER*)&m_i64StartCount);
	}
}

void CStopwatch::Stop()
{
	if(m_bIsRunning)
	{
		m_i64CulmativePauseCount = 0;
		m_i64PauseStartCount = 0;
		m_i64StartCount = 0;
		m_i64PreviousCount = 0;
		m_bIsPaused = false;
		m_bIsRunning = false;
	}
}

double CStopwatch::GetTimeElapsed()
{
	if (m_bIsRunning)
	{
		if (!m_bIsPaused)
		{
			__int64 i64ElapsedCount = 0;
			QueryPerformanceCounter((LARGE_INTEGER*)&i64ElapsedCount);
			m_i64PreviousCount = i64ElapsedCount;
			return ((i64ElapsedCount - m_i64StartCount - m_i64CulmativePauseCount) * m_dSecondsPerCount);
		}
		else
		{
			return ((m_i64PreviousCount - m_i64StartCount - m_i64CulmativePauseCount) * m_dSecondsPerCount);
		}
	}

	return 0.0;
}