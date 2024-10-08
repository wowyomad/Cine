#pragma once

#include "glash/glash_pch.hpp"
#include "glash/Core/Base.hpp"
#include "glash/Core/Input.hpp"
#include "glash/events/Event.hpp"

namespace Cine
{
	class GLASH_API MouseMovedEvent : public Event
	{
	public:

		MouseMovedEvent(float x, float y)
			: m_MouseX(x), m_MouseY(y) { }

		inline float GetMouseX() const { return m_MouseX; }
		inline float GetMouseY() const { return m_MouseY; }


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

		inline float GetHorizontal() const { return m_OffsetX; }
		inline float GetVertical() const { return m_OffsetY; }

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
		inline int GetMouseButton() const {	return m_Button; };

		EVENT_CLASS_CATEGORY(EventCategoryInput | EventCategoryMouse | EventCategoryMouseButton)
	protected:
		MouseCode m_Button;
		MouseButtonEvent(MouseCode button)
			: m_Button(button) { }
	};

	class GLASH_API MouseButtonPressedEvent : public MouseButtonEvent
	{
	public:
		MouseButtonPressedEvent(MouseCode button)
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
		MouseButtonReleasedEvent(MouseCode button)
			: MouseButtonEvent(button) { }

		inline std::string ToString() const override
		{
			return fmt::format("MouseButtonReleased: {}", m_Button);
		}

		EVENT_CLASS_TYPE(MouseButtonReleased)
	};
}

STRING_FORMAT(MouseButtonPressedEvent)
STRING_FORMAT(MouseButtonReleasedEvent)
STRING_FORMAT(MouseMovedEvent)
STRING_FORMAT(MouseScrolledEvent)