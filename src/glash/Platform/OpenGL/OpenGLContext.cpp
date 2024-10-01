#include "glash/Platform/OpenGL/OpenGLContext.hpp"

#include <GLFW/glfw3.h>
#include <glad/glad.h>

#include "glash/Renderer/Buffer.hpp"
#include "glash/VertexArray.hpp"
#include "glash/Renderer.hpp"

namespace glash
{
	GraphicsContext* GraphicsContext::Create(void *window)
	{
		return new OpenGLContext(static_cast<GLFWwindow*>(window));
	}

	OpenGLContext::OpenGLContext(GLFWwindow* windowHandle)
		:m_WindowHandle(windowHandle)
	{

	}

	void OpenGLContext::Init()
	{
		glfwMakeContextCurrent(m_WindowHandle);
		int status = gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress));
		GLASH_CORE_ASSERT(status, "Coudln't load glad");
		GLASH_CORE_INFO("OpenGL Vendor: {}", (const char*)glGetString(GL_VENDOR));
		GLASH_CORE_INFO("OpenGL Renderer: {}", (const char*)glGetString(GL_RENDERER));
		GLASH_CORE_INFO("OpenGL Version: {}", (const char*)glGetString(GL_VERSION));

		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	}
	void OpenGLContext::SwapBuffers()
	{
		glfwSwapBuffers(m_WindowHandle);
	}
}