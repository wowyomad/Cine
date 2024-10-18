#pragma once

#include "glash/Cine.hpp"

class ColorScript : public Cine::NativeScript
{
public:
	ColorScript()
	{
		++counter;
		CINE_LOG_INFO("Constructor for ColorScript");
	}
	void OnCreate() override
	{
		auto& component = GetComponent<Cine::SpriteRendererComponent>();
		m_SpriteRendererComponent = &component;
		m_Timer.Start();
	}

	void OnDestroy() override
	{
		CINE_LOG_WARN("Destroyed ColorScript");
	}

	void OnUpdate(Cine::Timestep ts) override
	{
		m_Timer.OnUpdate(ts);

		float time = m_Timer.GetElapsed();
		m_SpriteRendererComponent->Color.r = 0.5f * sin(time) + 0.5f;
		m_SpriteRendererComponent->Color.g = 0.5f * sin(time + 2.0f) + 0.5f;
		m_SpriteRendererComponent->Color.b = 0.5f * sin(time + 4.0f) + 0.5f;
	}
public:
	std::string String = "String from Color Script";

private:
	Cine::SpriteRendererComponent* m_SpriteRendererComponent;
	Cine::Timer m_Timer;
	static int counter;
};