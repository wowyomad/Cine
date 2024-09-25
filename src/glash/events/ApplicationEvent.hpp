#include "glash/Core.hpp"
#include "glash/glash_pch.hpp"

#include "glash/events/Event.hpp"

namespace glash
{
	class GLASH_API WindowResizeEvent : public Event
	{
	public:
		WindowResizeEvent(unsigned int width, unsigned int height)
			: m_Width(width), m_Height(height) { }

		inline unsigned int GetWidth() const { return m_Width; }
		inline unsigned int GetHeight() const { return m_Height; }

		inline std::string ToString() const override
		{
			return fmt::format("WindowResizeEvent: {}, {}", m_Width, m_Height);
		}
		EVENT_CLASS_TYPE(WindowResize)
		EVENT_CLASS_CATEGORY(EventCategoryApplication)

		inline EventType GetEventType() const override { return EventType(); }
		inline const char* GetName() const override { return nullptr; }
		inline int GetCategoryFlags() const override { return 0; }

	protected:
		unsigned int m_Width, m_Height;
	};
}