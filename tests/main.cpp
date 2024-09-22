#include "glash/glash_pch.hpp"

#include "TestClearColor.hpp"
#include "glash/ImguiWindow.hpp"
#include "glash/Renderer.hpp"

int main(int argc, char** argv)
{
	glash::ImguiWindow window(800, 800, "Test");

	float clearColor[4] = { 0.9f, 0.5f, 0.5f, 1.0f };

	glash::test::ClearColorTest cct(window, clearColor);


	while (!window.ShouldClose())
	{
		window.PollEvents();

		window.ClearBuffer();

		window.NewImguiFrame();

		cct.OnRender();		

		cct.OnImGuiRender();

		window.ImguiRender();
		
		window.SwapBuffers();
	}

}