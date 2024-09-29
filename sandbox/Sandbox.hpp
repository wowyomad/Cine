#pragma once

#include "glash/Glash.hpp"
#include "glash/Application.hpp"
#include "glash/Input.hpp"

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
		ImGuiContext* this_context = ImGui::GetCurrentContext();

		ImGui::Begin(m_Name.c_str());
		ImGui::Text("Fuck this shit");
		ImGui::End();
	}

private:
	char m_Buf[1024]{};


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

