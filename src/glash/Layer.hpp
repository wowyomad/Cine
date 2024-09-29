#pragma once

#include "glash/Core.hpp"
#include "glash/events/Event.hpp"

namespace glash
{
	class GLASH_API Layer
	{
	public:
		Layer(const std::string& name = "Layer");
		
		virtual ~Layer() = default;

		virtual void OnAttach();
		virtual void OnDetach() {}
		virtual void OnUpdate() {}
		virtual void OnImGuiRender() {}
		virtual void OnEvent(Event& event) {}

		inline const std::string& GetName() const { return m_Name; }
		inline std::string ToString() const { return GetName();  }

	protected:
		std::string m_Name;
	};
}
STRING_FORMAT(Layer)
