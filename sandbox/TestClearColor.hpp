#pragma once
#include "Test.hpp"
#include "glash/Window_OLD.hpp"
#include "glash/Enums.hpp"

namespace glash
{
	namespace test
	{
		class ClearColorTest : public Test
		{
		public:
			ClearColorTest(Window_OLD& window);
			ClearColorTest(Window_OLD& window, const Color& color);
			~ClearColorTest();

			virtual void OnUpdate(float deltaTime) override;
			virtual void OnRender() override;
			virtual void OnImGuiRender() override;

		private:
			Color m_ClearColor;
		};
	}
}