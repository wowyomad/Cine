#pragma once
#include "glash/glash_pch.hpp"
#include "glash/Core.hpp"

namespace glash
{
	enum class EventType
	{
		None = 0,
		WindowClose, WindowResize, WindowFocus, WindowLostFocus, WindowMoved,
		AppTick, AppUpdate, AppRender,
		KeyPressed, KeyReleased,
		MouseButtonPressed, MouseButtonReleased, MouseMoved, MouseScrolled
	};

	enum EventCategory
	{
		None = 0,
		EventCategoryApplication = BIT(0),
		EventCategoryInput = BIT(1),
		EventCategoryKeyboard = BIT(2),
		EventCategoryMouse = BIT(3),
		EventCategoryMouseButton = BIT(4)
	};

#define EVENT_CLASS_TYPE(type)\
	inline static EventType GetStaticEventType() { return EventType::##type; }\
	inline EventType GetEventType() const override { return GetStaticEventType(); }\
	inline const char* GetName() const override { return #type; }

#define EVENT_CLASS_CATEGORY(category)\
	inline int GetCategoryFlags() const override { return category; }

	class GLASH_API Event
	{
		friend class EventDispatcher;

	public:
		virtual EventType GetEventType() const = 0;
		virtual const char* GetName() const = 0;
		virtual int GetCategoryFlags() const = 0;
		virtual std::string ToString() const { return GetName(); }

		inline bool IsInCategory(EventCategory category)
		{
			return GetCategoryFlags() & category;
		}

		inline bool IsHandled() const { return m_Handled;  }

	protected:
		bool m_Handled = false;
	};

	template <typename T>
	concept EventDerived = std::derived_from<T, Event>;

	class GLASH_API EventDispatcher
	{

		template <class T>
		using EventFn = std::function<bool(T&)>;

	public:
		EventDispatcher(Event& event)
			: m_Event(event) { }

		template <EventDerived T>
		bool Dispatch(EventFn<T> func)
		{
			if (m_Event.GetEventType() == T::GetStaticEventType())
			{
				m_Event.m_Handled = func(static_cast<T&>(m_Event));
				return true;
			}
			return false;
		}

		
	
	private: 

		Event& m_Event;
	};

}

STRING_FORMAT(Event)