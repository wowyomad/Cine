#pragma once

#include "glash/Core/Core.hpp"
#include "glash/glash_pch.hpp"

#include "glash/events/Event.hpp"

namespace glash
{
	class GLASH_API ApplicationEvent : public Event
	{
	public:
		ApplicationEvent() {}
		
		EVENT_CLASS_CATEGORY(EventCategoryApplication)
	};

	class GLASH_API WindowCloseEvent : public ApplicationEvent
	{
	public:
		WindowCloseEvent() {}

		EVENT_CLASS_TYPE(WindowClose)
	};

	class GLASH_API WindowResizeEvent : public ApplicationEvent
	{
	public:
		WindowResizeEvent(unsigned int width, unsigned int height)
			: m_Width(width), m_Height(height) { }

		inline unsigned int Width() const { return m_Width; }
		inline unsigned int Height() const { return m_Height; }

		inline std::string ToString() const override
		{
			return fmt::format("WindowResizeEvent: {}, {}", m_Width, m_Height);
		}

		EVENT_CLASS_TYPE(WindowResize)

	protected:
		unsigned int m_Width, m_Height;
	};

	class GLASH_API WindowFocusEvent : public ApplicationEvent
	{
	public:
		WindowFocusEvent() {}

		EVENT_CLASS_TYPE(WindowFocus)
	};

	class GLASH_API WindowLostFocusEvent : public ApplicationEvent
	{
	public: 
		WindowLostFocusEvent() {}

		EVENT_CLASS_TYPE(WindowLostFocus)
	};

	class GLASH_API AppTickEvent : public ApplicationEvent
	{
		AppTickEvent() {}
		
		EVENT_CLASS_TYPE(AppTick)
	};

	class GLASH_API AppUpdateEvent : public ApplicationEvent
	{
		AppUpdateEvent() {}

		EVENT_CLASS_TYPE(AppUpdate)
	};

	class GLASH_API AppRenderEvent : public ApplicationEvent
	{
		AppRenderEvent() {}

		EVENT_CLASS_TYPE(AppRender)
	};
}

STRING_FORMAT(ApplicationEvent)
STRING_FORMAT(WindowCloseEvent)
STRING_FORMAT(WindowResizeEvent)
STRING_FORMAT(WindowFocusEvent)
STRING_FORMAT(WindowLostFocusEvent)
STRING_FORMAT(AppTickEvent)
STRING_FORMAT(AppUpdateEvent)
STRING_FORMAT(AppRenderEvent)