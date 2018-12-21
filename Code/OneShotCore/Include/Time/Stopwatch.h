#pragma once
#ifndef ONE_SHOT_CORE_STOPWATCH_H
#define ONE_SHOT_CORE_STOPWATCH_H

class ONE_SHOT_CORE_DLL CStopwatch
{
	public:
		CStopwatch();
		~CStopwatch();

		void Start();
		void Pause();
		void Resume();
		void Reset();
		void Stop();

		double GetTimeElapsed();

		bool IsPaused() const
		{
			return m_bIsPaused;
		}

		bool IsRunning() const
		{
			return m_bIsRunning;
		}

	private:
		bool m_bIsRunning, m_bIsPaused;
		__int64 m_i64StartCount, m_i64PauseStartCount, m_i64CulmativePauseCount, m_i64PreviousCount;
		double m_dSecondsPerCount;
};
#endif