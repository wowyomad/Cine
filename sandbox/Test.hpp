#pragma once

#include "glash/Core.hpp"
#include "glash/Window_OLD.hpp"

namespace glash
{
	namespace test
	{
		class Test
		{
		public:
			Test(Window_OLD& window) : m_Window(window), m_PreviuosColor(window.GetClearColor()) { }
			virtual ~Test() { m_Window.SetClearColor(m_PreviuosColor);  }

			virtual void OnUpdate(float deltaTime) {}
			virtual void OnRender() {}
			virtual void OnImGuiRender() {}

		protected:
			Window_OLD& m_Window;
			Color m_PreviuosColor;
		};
	}
}