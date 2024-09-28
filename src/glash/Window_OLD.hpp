#pragma once
#include "glash/glash_pch.hpp"

#include "glad/glad.h"
#include "GLFW/glfw3.h"

#include "glash/Core.hpp"
#include "glash/Enums.hpp"

namespace glash
{
	class Window_OLD;

	class GLASH_API Window_OLD
	{
	static Window_OLD* s_Window;

	public:
		using KeyCallback = std::function<void(GLFWwindow* window, int key, int scancode, int action, int mods)>;

		Window_OLD(int width, int height, const std::string& title, const Color& clearColor = color::GRAY);
		~Window_OLD();

		void PollEvents() const;
		bool ShouldClose() const;
		void ClearBuffer();
		void SwapBuffers() const;

		GLFWwindow* GetPtr() const;
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

		static void MainKeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
	};

} //namespace glash
