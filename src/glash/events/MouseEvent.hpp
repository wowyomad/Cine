#pragma once


#include "glash/Core.hpp"
#include "glash/glash_pch.hpp"

#include "glash/events/Event.hpp"

namespace glash
{
	class GLASH_API MouseMovedEvent : public Event
	{
	public:

		MouseMovedEvent(float x, float y)
			: m_MouseX(x), m_MouseY(y) { }

		inline std::string ToString() const override
		{
			return fmt::format("MouseMovedEvent: {}, {}", m_MouseX, m_MouseY);
		}

		EVENT_CLASS_TYPE(MouseMoved)
		EVENT_CLASS_CATEGORY(EventCategoryInput | EventCategoryMouse)

	protected:
		float m_MouseX, m_MouseY;
	};

	class GLASH_API MouseScrolledEvent : public Event
	{
	public:

		MouseScrolledEvent(float offsetX, float offsetY)
			: m_OffsetX(offsetX), m_OffsetY(offsetY) { }

		inline std::string ToString() const override
		{
			return fmt::format("MouseScrolledEvent: {}, {}", m_OffsetX, m_OffsetY);
		}

		EVENT_CLASS_TYPE(MouseScrolled)
		EVENT_CLASS_CATEGORY(EventCategoryInput | EventCategoryMouse)

	protected:
		float m_OffsetX, m_OffsetY;
	};

	class MouseButtonEvent : public Event
	{
	public:
		EVENT_CLASS_CATEGORY(EventCategoryInput | EventCategoryMouse | EventCategoryMouseButton)

	protected:
		int m_Button;
		MouseButtonEvent(int button)
			: m_Button(button) { }
	};

	class GLASH_API MouseButtonPressedEvent : MouseButtonEvent
	{
	public:
		MouseButtonPressedEvent(int button)
			: MouseButtonEvent(button) { }

		inline std::string ToString() const override
		{
			return fmt::format("MouseButtonPressed: {}", m_Button);
		}

		EVENT_CLASS_TYPE(MouseButtonPressed)
	};

	class GLASH_API MouseButtonReleasedEvent : public MouseButtonEvent
	{
	public:
		MouseButtonReleasedEvent(int button)
			: MouseButtonEvent(button) { }

		inline std::string ToString() const override
		{
			return fmt::format("MouseButtonReleased: {}", m_Button);
		}

		EVENT_CLASS_TYPE(MouseButtonReleased)
	};
}