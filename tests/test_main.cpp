#include "glash/glash_pch.hpp"

#include "TestClearColor.hpp"
#include "TestTexture.hpp"
#include "TestMenu.hpp"
#include "glash/ImguiWindow.hpp"
#include "glash/Renderer.hpp"


int main(int argc, char** argv)
{
	glash::ImguiWindow window(800, 800, "Test", glash::color::GRAY);

	glash::test::TestMenu testMenu (window);
	testMenu.RegisterTest<glash::test::ClearColorTest>("Clear Color Test", glash::Color(0.5, 0.6, 0.1, 1.0));
	testMenu.RegisterTest<glash::test::TestTexture>("Texture Test");



	while (!window.ShouldClose())
	{
		window.PollEvents();

		window.ClearBuffer();

		testMenu.OnRender();

		window.NewImguiFrame();

		testMenu.OnImGuiRender();

		window.ImguiRender();
		
		window.SwapBuffers();
	}

}