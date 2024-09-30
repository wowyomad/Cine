#pragma once


#include "glash/Core/Core.hpp"
#include "glash/glash_pch.hpp"

#include "glash/events/Event.hpp"

namespace glash
{
	class GLASH_API KeyEvent : public Event
	{
	public:
		inline KeyCode GetKeyCode() const { return m_KeyCode; }

		EVENT_CLASS_CATEGORY(EventCategoryInput | EventCategoryKeyboard)

	protected:
		KeyEvent(KeyCode keyCode)
			: m_KeyCode(keyCode) {}

		KeyCode m_KeyCode;
	};

	class  GLASH_API KeyPressedEvent : public KeyEvent
	{
	public:
		KeyPressedEvent(KeyCode keyCode, int repeatCount)
			: KeyEvent(keyCode), m_RepeatCount(repeatCount) { }

		inline int GetRepeatCount() const { return m_RepeatCount; }
		inline std::string ToString() const
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
		KeyReleasedEvent(KeyCode keyCode)
			: KeyEvent(keyCode) { }

		inline std::string ToString() const
		{
			return fmt::format("KeyReleasedEvent: {}", m_KeyCode);
		}

		EVENT_CLASS_TYPE(KeyReleased)

	};

	class  GLASH_API KeyTypedEvent : public KeyEvent
	{
	public:
		KeyTypedEvent(KeyCode keyCode)
			: KeyEvent(keyCode) { }

		inline std::string ToString() const
		{
			return fmt::format("KeyTypedEvent: {}", m_KeyCode);
		}

		EVENT_CLASS_TYPE(KeyTyped)
	};
}

STRING_FORMAT(KeyEvent)
STRING_FORMAT(KeyPressedEvent)
STRING_FORMAT(KeyReleasedEvent)
