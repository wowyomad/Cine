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

	void OnUpdate(Cine::Timestep ts) override;
	
public:
	std::string String = "String from Color Script";

private:
	Cine::SpriteRendererComponent* m_SpriteRendererComponent;
	Cine::Timer m_Timer;
	static int counter;
};