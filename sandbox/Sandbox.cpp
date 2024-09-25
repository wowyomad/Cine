#include "Sandbox.hpp"

#include "glash/Core.hpp"
#include "glash/Glash.hpp"

glash::Application* glash::CreateApplication()
{
	GLASH_TRACE("Trace inside {}", __FUNCTION__);
	GLASH_DEBUG("Debug inside {}", __FUNCTION__);
	GLASH_INFO("Info inside {}", __FUNCTION__);
	GLASH_ERROR("Error inside {}", __FUNCTION__);

	return new Sandbox();
}



//#include "GLFW/glfw3.h"
//#include "glash/glash_pch.hpp"
//#include "TestClearColor.hpp"
//#include "TestTexture.hpp"
//#include "TestMenu.hpp"
//#include "glash/Renderer.hpp"
//
//#include "glash/Glash.hpp"

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