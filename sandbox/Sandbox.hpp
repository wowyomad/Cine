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
		bool space = glash::Input::IsKeyPressed(glash::Key::Space);

		if (space)
		{
			GLASH_LOG_INFO("Space was pressed");
		}

		if (glash::Input::IsMouseButtonPressed(glash::Mouse::ButtonLeft))
		{
			GLASH_LOG_INFO("LMB was pressed");
		}

		if (glash::Input::IsMouseButtonPressed(glash::Mouse::ButtonRight))
		{
			GLASH_LOG_INFO("RMB was pressed");
		}

		if (glash::Input::IsMouseButtonPressed(glash::Mouse::ButtonMiddle))
		{
			GLASH_LOG_INFO("MMB was pressed");
		}

		//auto [x, y] = glash::Input::GetMouseXY();
		//GLASH_CORE_TRACE("Position: {}, {}", x, y);
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

