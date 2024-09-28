#pragma once
#include "glash/Core.hpp"

#include "glash/events/Event.hpp"


namespace glash
{
	class GLASH_WINDOW_CLASS;

	struct WindowProps
	{
		std::string Title;
		unsigned int Width;
		unsigned int Height;

		WindowProps(
			const std::string& title = "eNgInE",
			unsigned int width = 1280,
			unsigned int height = 720)
			: Title(title), Width(width), Height(height) {}
	};

	class GLASH_API Window
	{
	public:
		using EventCallbackFn = std::function<void(Event&)>;

		Window() = default;
		virtual ~Window() = default;
		// Direct method declarations
		void OnUpdate(); // Implement in one of the classes.
		unsigned int Height() const; // Implement in one of the classes.
		unsigned int Width() const; // Implement in one of the classes.
		void SetEventCallback(const EventCallbackFn& callback); // Implement in one of the classes.
		void AddEventCallback(const EventCallbackFn& callback); // Implement in one of the classes.
		void SetVSync(bool enabled); // Implement in one of the classes.
		bool IsVSync() const; // Implement in one of the classes.

		// Factory method to create windows
		static GLASH_WINDOW_CLASS* Create(const WindowProps& props = WindowProps());
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