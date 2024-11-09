#pragma once
#include "include/ScriptCore.hpp"

using namespace Cine;

class ColorScript : public NativeScript
{
public:
	ColorScript()
	{
		m_Timer.Start();
	}

	ColorScript& operator=(const ColorScript& other)
	{
		m_Time = other.m_Time;
		m_String = other.m_String;
		return *this;
	}

	void OnCreate() override
	{
		m_Timer.Start();
		std::cout << "OnCreate: " << m_Timer.GetElapsed() << std::endl;
		m_SpriteComponent = TryGetComponent<SpriteComponent>();
	}

	void OnUpdate(Timestep ts) override
	{
		if (!m_SpriteComponent)
		{
			return;
		}

		m_Timer.OnUpdate(ts);

		m_Time = m_Timer.GetElapsed();
		m_SpriteComponent->Color.r = 0.5f * sin(m_Time) + 0.5f;
		m_SpriteComponent->Color.g = 0.5f * sin(m_Time + 2.0f) + 0.5f;
		m_SpriteComponent->Color.b = 0.5f * sin(m_Time + 4.0f) + 0.5f;
	}

	void OnTriggerEnter(Entity entity) override
	{
		
	}

	void OnTriggerExit(Entity entity) override
	{

	}
	
public:
	SERIALIZE_CLASS(ColorScript,
		FIELD(m_Time)
		FIELD(m_String)
	)

private:
	SpriteComponent* m_SpriteComponent = nullptr;
	Timer m_Timer;
	float m_Time = 0.0f;
	std::string m_String = "String from Color Script";

};