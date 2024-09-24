#pragma once


#include "Test.hpp"
#include "glash/Window.hpp"
#include "glash/Enums.hpp"

namespace glash
{
	namespace test
	{
		class ClearColorTest : public Test
		{
		public:
			ClearColorTest(Window& window);
			ClearColorTest(Window& window, const Color& color);
			~ClearColorTest();

			virtual void OnUpdate(float deltaTime) override;
			virtual void OnRender() override;
			virtual void OnImGuiRender() override;

		private:
			Color m_ClearColor;
		};
	}
}