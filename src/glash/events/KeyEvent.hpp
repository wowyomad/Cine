#pragma once


#include "glash/Core.hpp"
#include "glash/glash_pch.hpp"

#include "glash/events/Event.hpp"

namespace glash
{
	class GLASH_API KeyEvent : public Event
	{
	public:
		inline int GetKeyCode() const { return m_KeyCode; }

		EVENT_CLASS_CATEGORY(EventCategoryInput | EventCategoryKeyboard)

	protected:
		KeyEvent(int keyCode)
			: m_KeyCode(keyCode) {}

		int m_KeyCode;
	};

	class  GLASH_API KeyPressedEvent : public KeyEvent
	{
	public:
		KeyPressedEvent(int keyCode, int repeatCount)
			: KeyEvent(keyCode), m_RepeatCount(repeatCount) { }

		inline int GetRepeatCount() const { return m_RepeatCount; }
		inline std::string Tostring() const
		{
			return fmt::format("KeyPressedEvent: {} ({} repeats)", m_KeyCode, m_RepeatCount);
		}

		EVENT_CLASS_TYPE(KeyPressed)
		
	protected:
		int m_RepeatCount;
	};

	class  GLASH_API KeyReleasedEvent : public KeyEvent
	{
	public:
		KeyReleasedEvent(int keyCode)
			: KeyEvent(keyCode) { }

		inline std::string Tostring() const
		{
			return fmt::format("KeyReleasedEvent: {}", m_KeyCode);
		}

		EVENT_CLASS_TYPE(KeyReleased)

	};
}