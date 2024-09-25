#include "Sandbox.hpp"

#include "GLFW/glfw3.h"
#include "glash/glash_pch.hpp"
#ifdef GLASH_EXPORT
#undef GLASH_EXPORT
#endif

#include "TestClearColor.hpp"
#include "TestTexture.hpp"
#include "TestMenu.hpp"
#include "glash/Renderer.hpp"
//#include "glash/Logger.hpp"
//#include "glash/Core.hpp"
//#include "ImguiWindow.hpp"

#include "glash/Glash.hpp"


glash::Application* glash::CreateApplication()
{
	return new Sandbox();
}

//int main(int argc, char** argv)
//{
//	LOG_INFO("BUILD_STR on sandbox side: {}", BUILD_STR);
//	glash::Window window(800, 800, "Test", glash::color::GRAY);
//	ImguiWindow imguiWindow(window);
//	glash::test::TestMenu testMenu (window);
//
//	testMenu.RegisterTest<glash::test::ClearColorTest>("Clear Color Test", glash::Color(1.0, 0.6, 0.1, 1.0));
//	testMenu.RegisterTest<glash::test::TestTexture>("Texture Test");
//
//
//	float time = 0.001f;
//	while (!window.ShouldClose())
//	{
//		window.PollEvents();
//
//		float newTime = glfwGetTime();
//		float deltaTime = newTime - time;
//		time = newTime;
//		testMenu.OnUpdate(deltaTime);
//
//		window.ClearBuffer();
//
//		testMenu.OnRender();
//
//		imguiWindow.NewImguiFrame();
//
//		testMenu.OnImGuiRender();
//
//		imguiWindow.ImguiRender();
//		
//		window.SwapBuffers();
//	}
//}