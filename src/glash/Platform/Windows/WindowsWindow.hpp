#pragma once
#include "glash/Core/Core.hpp"
#include "glash/Core/Window.hpp"

#include "glash/Renderer/GrahpicsContext.hpp"

struct GLFWwindow;

namespace Cine
{
	class GLASH_API WindowsWindow : public IWindow
	{
	public:
		WindowsWindow(const WindowProps& props);
		~WindowsWindow();

		void OnUpdate();

		inline unsigned int GetWidth() const override final { return m_Data.Width; }
		inline unsigned int GetHeight() const override final { return m_Data.Height; }

		void SetEventCallback(const EventCallbackFn& callback) override final;
		void AddEventCallback(const EventCallbackFn& callback) override final;

		void SetTitle(const std::string& title) override final;
		void SetVSync(bool enabled);
		bool IsVSync() const override final;
		void* GetNativeWindow() const override final;


	private:
		void Init(const WindowProps& props);
		void Shutdown();
		void InitEventCallbacks();

		static bool s_GLFWinitialized;

	protected:
		GLFWwindow* m_Window;
		GraphicsContext* m_Context;

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