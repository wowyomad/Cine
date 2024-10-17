#pragma once
#include "glash/Core/Log.hpp"

#include <chrono>

namespace Cine
{
	template <class Fn>
	class BenchmarkTimer
	{

	public:
		BenchmarkTimer(const char* name, Fn&& resultCallback)
			: m_Name(name), m_ResultCallback(resultCallback)
		{
			Reset();
		}

		void Reset()
		{
			m_Start = std::chrono::high_resolution_clock::now();
		}

		float Elapsed()
		{
			return std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::high_resolution_clock::now() - m_Start).count() * 1e-9f;
		}

		float ElapsedMillis()
		{
			return Elapsed() * 1000.0f;
		}

		void Stop()
		{
			float elapsed = ElapsedMillis();
			m_Stopped = true;
	
			m_ResultCallback({ m_Name, elapsed });
		}

		~BenchmarkTimer()
		{
			if (!m_Stopped)
			{
				Stop();
			}
		}

	private:
		Fn m_ResultCallback;
		bool m_Stopped = false;
		const char* m_Name;
		std::chrono::time_point<std::chrono::high_resolution_clock> m_Start;

	};

	class Timer
	{
	public:
		Timer() : m_Started(false), m_ElapsedTime(0.0f) {}

		void Start()
		{
			m_StartTimePoint = std::chrono::steady_clock::now();
			m_Started = true;
			m_ElapsedTime = 0.0f;
		}

		void OnUpdate(float ts)
		{
			if (m_Started)
			{
				m_ElapsedTime += ts; 
			}
		}

		std::chrono::steady_clock::time_point GetTimeStarted() const
		{
			return m_StartTimePoint;
		}

		float GetElapsed() const
		{
			return m_ElapsedTime;
		}

		// Stops the timer
		void Stop()
		{
			m_Started = false;
		}

		// Checks if the timer is currently running
		bool IsRunning() const
		{
			return m_Started;
		}

	private:
		bool m_Started;
		float m_ElapsedTime; // Accumulated elapsed time in seconds
		std::chrono::steady_clock::time_point m_StartTimePoint;
	};
}
