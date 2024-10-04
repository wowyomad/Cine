#pragma once

namespace glash {

	class Timestep
	{
	public:
		Timestep(double time = 0.0f)
			: m_Time(time) { }

		operator float() const { return m_Time; }

		inline float Seconds() const { return m_Time; }
		inline float Milleseconds() const { return m_Time * 1000.0f; }
	private:
		double m_Time;
	};

}