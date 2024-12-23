#pragma once

#include "glash/Core/Base.hpp"
#include "glash/Core/Layer.hpp"
#include "glash/events/MouseEvent.hpp"
#include "glash/events/KeyEvent.hpp"
#include "glash/events/ApplicationEvent.hpp"

#include "imgui.h"

namespace Cine
{
	class GLASH_API ImGuiLayer : public Layer
	{
	public:
		ImGuiLayer(const std::string& name);
		ImGuiLayer();
		~ImGuiLayer();

		void OnAttach() override final;
		void OnDetach() override final;
		void OnEvent(Event& e) override final;

		void SetBlockEvents(bool blockEvents);

		void SetDarkTheme();
		void SetLightTheme();

		void Begin();
		void End();
		 
	private:
		bool m_BlockEvents = false;
	};
}