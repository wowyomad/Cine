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

		glm::vec2 pos = glash::Input::GetMousePosition();
		GLASH_CORE_TRACE("Position: {}, {}", pos.x, pos.y);
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

