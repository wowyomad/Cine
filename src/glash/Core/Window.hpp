#pragma once
#include "glash/glash_pch.hpp"

#include "glash/events/Event.hpp"


namespace Cine
{
	class GLASH_WINDOW_CLASS;
	class IWindow;
	using Window = IWindow;

	struct WindowProps
	{
		std::string Title;
		unsigned int Width;
		unsigned int Height;
		bool VSync;

		WindowProps(
			const std::string& title = "eNgInE",
			unsigned int width = 1280,
			unsigned int height = 720,
			bool vsync = false)
			: Title(title), Width(width), Height(height), VSync(vsync) { }
	};

	class GLASH_API IWindow
	{
	public:
		using EventCallbackFn = std::function<void(Event&)>;

		IWindow() = default;
		virtual ~IWindow() = default;
		
		virtual void OnUpdate() = 0;
		virtual void SetEventCallback(const EventCallbackFn& callback) = 0;
		virtual void AddEventCallback(const EventCallbackFn& callback) = 0;
		virtual void SetVSync(bool enabled) = 0;
		virtual void SetTitle(const std::string& title) = 0;
		virtual bool IsVSync() const = 0;
		virtual void* GetNativeWindow() const = 0;

		virtual unsigned int GetWidth() const = 0;
		virtual unsigned int GetHeight() const = 0;

		// Factory method to create windows
		static Ref<Window> Create(const WindowProps& props = WindowProps());
	};

	/*class GLASH_API Window
	{
	public:
		using EventCallbackFn = std::function<void(Event&)>;

		inline Window() { GLASH_LOG_INFO(BUILD_STR); }
		virtual ~Window() {}

		virtual void OnUpdate() = 0;

		virtual unsigned int Height() const = 0;
		virtual unsigned int Width() const = 0;

		virtual void SetEventCallback(const EventCallbackFn& callback) = 0;
		virtual void AddEventCallback(const EventCallbackFn& callback) = 0;
		virtual void SetVSync(bool enabled) = 0;
		virtual bool IsVSync() const = 0;

		static Window* Create(const WindowProps& props = WindowProps());
	};*/
}