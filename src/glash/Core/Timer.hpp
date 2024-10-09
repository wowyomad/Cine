#pragma once
#include "glash/Core/Log.hpp"

#include <chrono>

namespace Cine
{
	template <class Fn>
	class Timer
	{

	public:
		Timer(const char* name, Fn&& resultCallback)
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

		~Timer()
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
}
