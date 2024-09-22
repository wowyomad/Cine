#include "Test.hpp"
#include "glash/Window.hpp"

namespace glash
{
	namespace test
	{
		class ClearColorTest : Test
		{
		public:
			ClearColorTest(Window& window, const float color[4]);
			~ClearColorTest();

			virtual void OnUpdate(float deltaTime) override;
			virtual void OnRender() override;
			virtual void OnImGuiRender() override;

		private:
			float m_ClearColor[4];
			Window& m_Window;
		};
	}
}