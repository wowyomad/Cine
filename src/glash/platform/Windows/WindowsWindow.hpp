#pragma once
#include "glash/Core/Core.hpp"
#include "glash/Core/Window.hpp"

struct GLFWwindow;

namespace glash
{
	class GLASH_API WindowsWindow : public Window
	{
	public:
		WindowsWindow(const WindowProps& props);
		~WindowsWindow();

		// Implement the methods directly
		void OnUpdate();
		unsigned int Width() const { return m_Data.Width; }
		unsigned int Height() const { return m_Data.Height; }

		void SetEventCallback(const EventCallbackFn& callback);
		void AddEventCallback(const EventCallbackFn& callback);

		void SetVSync(bool enabled);
		bool IsVSync() const;
		void* GetNativeWindow() const;


	private:
		void Init(const WindowProps& props);
		void Shutdown();
		void InitEventCallbacks();

		static bool s_GLFWinitialized;

	protected:
		GLFWwindow* m_Window;

		struct WindowData
		{
			std::string Title;
			unsigned int Width, Height;
			bool VSync;

			EventCallbackFn EventCallback;
		};

		WindowData m_Data;
	};
	/*class GLASH_API WindowsWindow : public Window
	{
	public:
		WindowsWindow(const WindowProps& props);
		~WindowsWindow();

		void OnUpdate() override;

		inline unsigned int Width() const override { return m_Data.Width; }
		inline unsigned int Height() const override { return m_Data.Height; }

		void SetEventCallback(const EventCallbackFn& callback) override;
		void AddEventCallback(const EventCallbackFn& callback) override;

		void SetVSync(bool enabled) override;
		bool IsVSync() const override;

	private:
		virtual void Init(const WindowProps& props);
		virtual void Shutdown();

		void InitEventCallbacks();

		static bool s_GLFWinitialized;

	protected:
		GLFWwindow* m_Window;

		struct WindowData
		{
			std::string Title;
			unsigned int Width, Height;
			bool VSync;

			EventCallbackFn EventCallback;
		};

		WindowData m_Data;
	};*/
}