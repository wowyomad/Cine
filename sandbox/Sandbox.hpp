#pragma once
#include "glash/Glash.hpp"
#include "glash/Input.hpp"

class SimpleLayer : public glash::Layer
{
public:
	SimpleLayer()
		: Layer("Simple Sandbox Layer") {}


	void OnUpdate() override
	{
		if (glash::Input::IsKeyPressed(glash::Key::Space))
		{
			GLASH_LOG_INFO("Space was pressed");
		}

		auto [x, y] = glash::Input::GetMouseXY();
		GLASH_CORE_TRACE("Position: {}, {}", x, y);
	}

	void OnEvent(glash::Event& event) override
	{
		
	}


};

class Sandbox : public glash::Application
{
public:
	Sandbox()
	{
		PushLayer(new SimpleLayer());
		PushLayer(new glash::ImGuiLayer());
	}

	~Sandbox()
	{
			
	}
};

