#pragma once

#include "glash/Window.hpp"

namespace glash
{
	namespace test
	{
		class Test
		{
		public:
			Test(Window& window) : m_Window(window) {}
			virtual ~Test() {}

			virtual void OnUpdate(float deltaTime) {}
			virtual void OnRender() {}
			virtual void OnImGuiRender() {}

		protected:
			Window& m_Window;
		};
	}
}