#pragma once
#include "glash/glash_pch.hpp"

#include "glash/events/Event.hpp"


namespace glash
{
	class GLASH_WINDOW_CLASS;

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

	class GLASH_API Window
	{
	public:
		using EventCallbackFn = std::function<void(Event&)>;

		Window() = default;
		virtual ~Window() = default;
		
		void OnUpdate();
		unsigned int GetHeight() const;
		unsigned int GetWidth() const;
		void SetEventCallback(const EventCallbackFn& callback);
		void AddEventCallback(const EventCallbackFn& callback);
		void SetVSync(bool enabled);
		void SetTitle(const std::string& title);
		bool IsVSync() const;
		void* GetNativeWindow() const;

		// Factory method to create windows
		static Ref<GLASH_WINDOW_CLASS> Create(const WindowProps& props = WindowProps());
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