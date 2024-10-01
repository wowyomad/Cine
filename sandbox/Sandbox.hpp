#pragma once

#include "glash/Glash.hpp"
#include "glash/Core/Application.hpp"
#include "glash/Core/Input.hpp"

class SimpleLayer : public glash::Layer
{
public:
	SimpleLayer()
		: Layer("Simple Sandbox Layer") {
		GLASH_LOG_TRACE(BUILD_STR);
	}


	void OnUpdate() override
	{

	}

	void OnEvent(glash::Event& event) override
	{

	}

	void OnImGuiRender() override
	{
		if (m_Show)
		{
			ImGui::Begin(m_Name.c_str(), &m_Show);
			ImGui::Text("Fuck this shit");
			ImGui::InputText("Input: ", m_Buf, sizeof(m_Buf));
			ImGui::End();
		}

	}

private:
	char m_Buf[1024]{};
	bool m_Show = true;

};

class Sandbox : public glash::Application
{
public:
	Sandbox()
	{
		GLASH_LOG_TRACE(BUILD_STR);
		PushOverlay(new SimpleLayer());
	}

	~Sandbox()
	{

	}
};

