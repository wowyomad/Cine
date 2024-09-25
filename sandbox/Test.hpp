#pragma once

#include "glash/Core.hpp"
#include "glash/Window.hpp"

namespace glash
{
	namespace test
	{
		class GLASH_API Test
		{
		public:
			Test(Window& window) : m_Window(window), m_PreviuosColor(window.GetClearColor()) { }
			virtual ~Test() { m_Window.SetClearColor(m_PreviuosColor);  }

			virtual void OnUpdate(float deltaTime) {}
			virtual void OnRender() {}
			virtual void OnImGuiRender() {}

		protected:
			Window& m_Window;
			Color m_PreviuosColor;
		};
	}
}