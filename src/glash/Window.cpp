#include "glash/glash_pch.hpp"
#include "Window.hpp"

#include "glash/Log.hpp"

namespace glash
{
	Window* Window::s_Window = nullptr;

	Window::Window(int width, int height, const std::string& title, const Color& clearColor)
		: m_ClearColor(clearColor)
	{
		assert(s_Window == nullptr && "Only works for one window for now!");
		s_Window = this;

		if (!glfwInit())
		{
			throw std::runtime_error("Failed to initialize GLFW");
		}
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
		glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

		m_pWindow = glfwCreateWindow(width, height, title.c_str(), nullptr, nullptr);
		if (!m_pWindow)
		{
			glfwTerminate();
			throw std::runtime_error("Failed to create GLFW window");
		}

		glfwMakeContextCurrent(m_pWindow);

		glfwSetWindowSizeCallback(m_pWindow, [](GLFWwindow* window, int width, int height)
			{
				glViewport(0, 0, width, height);
			});
		glfwSetKeyCallback(m_pWindow, MainKeyCallback);


		GLenum err = glewInit();
		if (GLEW_OK != err)
		{
			throw std::runtime_error("Failed to initialize GLEW");
		}
		fmt::println("OpenGL Version: {}\n", reinterpret_cast<const char*>(glGetString(GL_VERSION)));

		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glBlendEquation(GL_FUNC_ADD);

		glash::debug::InitializeOpenGLDebug();

	}

	Window::~Window()
	{
		glfwDestroyWindow(m_pWindow);
		glfwTerminate();
	}

	void Window::PollEvents() const
	{
		glfwPollEvents();
	}

	bool Window::ShouldClose() const
	{
		return glfwWindowShouldClose(m_pWindow);
	}

	void Window::ClearBuffer()
	{
		glClearColor(m_ClearColor.r, m_ClearColor.g, m_ClearColor.b, m_ClearColor.y);
		glClear(GL_COLOR_BUFFER_BIT);
	}

	void Window::SwapBuffers() const
	{
		glfwSwapBuffers(m_pWindow);
	}

	GLFWwindow* Window::GetPtr() const
	{

		return m_pWindow;
	}

	std::string Window::GetGLVersion() const
	{
		const char* version = reinterpret_cast<const char*>(glGetString(GL_VERSION));
		int major, minor;
		sscanf(version, "%d.%d", &major, &minor);
		int versionNumber = major * 100 + minor * 10;

		return std::string("#version ") + std::to_string(versionNumber);
	}

	void Window::GetWindowSize(int* w, int* h) const
	{
		glfwGetWindowSize(m_pWindow, w, h);
	}


	void Window::AddKeyListener(const void* owner, KeyCallback callback)
	{
		m_KeyCallbacks[(size_t)owner] = callback;
	}

	void Window::RemoveKeyListener(const void* owner)
	{
		auto it = m_KeyCallbacks.find((size_t)owner);
		if (it != m_KeyCallbacks.end())
		{
			m_KeyCallbacks.erase(it);
		}
	}

	void Window::SetClearColor(const Color& color)
	{
		m_ClearColor = color;
	}

	Color Window::GetClearColor() const
	{
		return m_ClearColor;
	}

	void Window::MainKeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
	{
		for (auto& [id, callback] : s_Window->m_KeyCallbacks)
		{
			callback(window, key, scancode, action, mods);
		}
	}


} // namespace glash
