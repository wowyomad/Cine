#pragma once
#include "include/ScriptCore.hpp"

using namespace Cine;

class ColorScript : public NativeScript
{
public:
	ColorScript()
	{

	}

	ColorScript& operator=(const ColorScript& other)
	{
		if (this == &other)
			return *this;

		m_Time = other.m_Time;
		m_String = other.m_String;

		NativeScript::operator=(other);
		return *this;
	}

	void OnCreate() override
	{
		
	}

	void OnUpdate(Timestep ts) override
	{
		SpriteComponent* sc = TryGetComponent<SpriteComponent>();
		if (!sc)
		{
			return;
		}

		m_Time += ts;
		sc->Color.r = 0.5f * sin(m_Time) + 0.5f;
		sc->Color.g = 0.5f * sin(m_Time + 2.0f) + 0.5f;
		sc->Color.b = 0.5f * sin(m_Time + 4.0f) + 0.5f;
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
	float m_Time = 0.0f;
	std::string m_String = "String from Color Script";

};