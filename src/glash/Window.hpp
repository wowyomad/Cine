#pragma once
#include "glash/glash_pch.hpp"
#include <GLFW/glfw3.h>

#include "glash/Enums.hpp"
#include <functional>


namespace glash
{
	class Window;

	extern Window* g_Window;

	class Window
	{

	public:
		using KeyCallback = std::function<void(GLFWwindow* window, int key, int scancode, int action, int mods)>;

		Window(int width, int height, const std::string& title, const Color& clearColor = color::GRAY);
		~Window();

		void PollEvents() const;
		bool ShouldClose() const;
		void ClearBuffer();
		void SwapBuffers() const;

		GLFWwindow* GetPtr();
		std::string GetGLVersion() const;

	public:
		void GetWindowSize(int* w, int* h) const;
		void AddKeyListener(const void* owner, KeyCallback callback);
		void RemoveKeyListener(const void* owner);

		void SetClearColor(const Color& color);
		Color GetClearColor() const;
	protected:
		GLFWwindow* m_pWindow;
		Color m_ClearColor;

		std::map<size_t, KeyCallback> m_KeyCallbacks;

		inline static void MainKeyCallback (GLFWwindow* window, int key, int scancode, int action, int mods)
			{
			for (auto& [id, callback] : g_Window->m_KeyCallbacks)
				{
					callback(window, key, scancode, action, mods);
				}
			};
	};

} //namespace glash
