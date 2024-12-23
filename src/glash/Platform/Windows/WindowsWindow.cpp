#include "glash/glash_pch.hpp"
#include "WindowsWindow.hpp"

#include "glash/Core/Log.hpp"
#include "glash/Core/Input.hpp"

#include "glash/events/ApplicationEvent.hpp"
#include "glash/events/KeyEvent.hpp"
#include "glash/events/MouseEvent.hpp"

#include <glad/glad.h>
#include <GLFW/glfw3.h>


#if CINE_PLATFORM_WINDOWS == 1
namespace Cine
{
	bool WindowsWindow::s_GLFWinitialized = false;

	static void GLFWErrorCallback(int error, const char* description)
	{
		CINE_CORE_ERROR("GLFW error {}: {}", error, description);
	}

	Ref<Window> IWindow::Create(const WindowProps& props)
	{
		//TODO: Choose correct one
		return CreateRef<WindowsWindow>(props);
	}

	WindowsWindow::WindowsWindow(const WindowProps& props)
	{
		Init(props);
	}

	WindowsWindow::~WindowsWindow()
	{
		Shutdown();
	}

	unsigned int WindowsWindow::GetPositionX() const
	{
		int x, y;
		glfwGetWindowPos(m_Window, &x, &y);
		return x;
	}
	unsigned int WindowsWindow::GetPositionY() const
	{
		int x, y;
		glfwGetWindowPos(m_Window, &x, &y);
		return y;
	}

	void WindowsWindow::SetEventCallback(const EventCallbackFn& callback)
	{
		m_Data.EventCallback = callback;
	}
	void WindowsWindow::AddEventCallback(const EventCallbackFn& callback)
	{
		SetEventCallback(callback);
	}
	void WindowsWindow::SetTitle(const std::string& title)
	{
		m_Data.Title = title;
		glfwSetWindowTitle(m_Window, title.c_str());
	}
	void WindowsWindow::SetVSync(bool enabled)
	{
		if (enabled)
		{
			glfwSwapInterval(1);
		}
		else
		{
			glfwSwapInterval(0);
		}

		m_Data.VSync = enabled;
	}
	bool WindowsWindow::IsVSync() const
	{

		return false;
	}
	void* WindowsWindow::GetNativeWindow() const
	{
		return m_Window;
	}
	void WindowsWindow::Init(const WindowProps& props)
	{
		m_Data.Width = props.Width;
		m_Data.Height = props.Height;
		m_Data.Title = props.Title;
		m_Data.VSync = props.VSync;

		CINE_CORE_INFO("Creating Windows window {} ({}, {}), VSync: {}", props.Title, props.Width, props.Height, m_Data.VSync ? "On" : "Off");


		if (!s_GLFWinitialized)
		{
			int success = glfwInit();
			CINE_CORE_ASSERT(success, "because fuck you");
			s_GLFWinitialized = true;
		}	
		glfwSetErrorCallback(GLFWErrorCallback);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);

		m_Window = glfwCreateWindow(static_cast<int>(m_Data.Width), static_cast<int>(m_Data.Height), m_Data.Title.c_str(), nullptr, nullptr);

		m_Context = GraphicsContext::Create(m_Window);
		m_Context->Init();

		glfwSetWindowUserPointer(m_Window, &m_Data);
		SetVSync(m_Data.VSync);

		InitEventCallbacks();
	}

	void WindowsWindow::OnUpdate()
	{
		glfwPollEvents();
		m_Context->SwapBuffers();
	}

	void WindowsWindow::Shutdown()
	{

	}

	void WindowsWindow::InitEventCallbacks()
	{
		glfwSetWindowSizeCallback(m_Window, [](GLFWwindow* window, int width, int height)
			{
				WindowData& data = *static_cast<WindowData*>(glfwGetWindowUserPointer(window));

				data.Width = width;
				data.Height = height;

				WindowResizeEvent event(width, height);
				data.EventCallback(event);

				glViewport(0, 0, width, height); //TODO: this shouldn't be here
 			});
		glfwSetWindowCloseCallback(m_Window, [](GLFWwindow* window)
			{
				WindowData& data = *static_cast<WindowData*>(glfwGetWindowUserPointer(window));

				WindowCloseEvent event;
				data.EventCallback(event);
			});
		glfwSetWindowFocusCallback(m_Window, [](GLFWwindow* window, int focus)
			{
				WindowData& data = *static_cast<WindowData*>(glfwGetWindowUserPointer(window));

				if (focus)
				{
					WindowFocusEvent focusEvent;
					data.EventCallback(focusEvent);
				}
				else
				{
					WindowLostFocusEvent lostFocusEvent;
					data.EventCallback(lostFocusEvent);
				}
			});
		glfwSetWindowPosCallback(m_Window, [](GLFWwindow* window, int xpos, int ypos)
			{
				WindowData& data = *static_cast<WindowData*>(glfwGetWindowUserPointer(window));
			});
		glfwSetKeyCallback(m_Window, [](GLFWwindow* window, int glfwKey, int scancode, int action, int mods)
			{
				WindowData& data = *static_cast<WindowData*>(glfwGetWindowUserPointer(window));
				KeyCode key = Internal::Input::ToCineKey(glfwKey);

				switch (action)
				{
				case GLFW_RELEASE:
				{
					Internal::Input::SetKey(key, Internal::Input::KeyState::Up);
					KeyReleasedEvent event(key);
					data.EventCallback(event);
				} break;
				case GLFW_PRESS:
				{
					Internal::Input::SetKey(key, Internal::Input::KeyState::Down);
					KeyPressedEvent event(key, 0);
					data.EventCallback(event);
				} break;
				case GLFW_REPEAT:
				{
					KeyPressedEvent event(key, 1);
					data.EventCallback(event);
				} break;
				}
			});
		glfwSetCharCallback(m_Window, [](GLFWwindow* window, unsigned int keycode)
			{
				WindowData& data = *static_cast<WindowData*>(glfwGetWindowUserPointer(window));
				
				KeyTypedEvent event(static_cast<int>(keycode));
				data.EventCallback(event);
			});

		glfwSetMouseButtonCallback(m_Window, [](GLFWwindow* window, int glfwButton, int action, int mods)
			{
				WindowData& data = *static_cast<WindowData*>(glfwGetWindowUserPointer(window));
				MouseCode button = Internal::Input::ToCineMouse(glfwButton);

				switch (action)
				{
				case GLFW_RELEASE:
				{
					Internal::Input::SetMouseButton(button, Internal::Input::KeyState::Up);
					MouseButtonReleasedEvent event(button);
					data.EventCallback(event);
				} break;
				case GLFW_PRESS:
				{
					Internal::Input::SetMouseButton(button, Internal::Input::KeyState::Down);
					MouseButtonPressedEvent event(button);
					data.EventCallback(event);
				} break;
				}
			});
		glfwSetScrollCallback(m_Window, [](GLFWwindow* window, double xoffset, double yoffset)
			{
				WindowData& data = *static_cast<WindowData*>(glfwGetWindowUserPointer(window));

				MouseScrolledEvent event(static_cast<float>(xoffset), static_cast<float>(yoffset));
				data.EventCallback(event);
			});
		glfwSetCursorPosCallback(m_Window, [](GLFWwindow* window, double xpos, double ypos)
			{
				WindowData& data = *static_cast<WindowData*>(glfwGetWindowUserPointer(window));
				
				MouseMovedEvent event(static_cast<float>(xpos), static_cast<float>(ypos));
				data.EventCallback(event);
			});

	}
}
#endif