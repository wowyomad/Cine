#pragma once

#include "glash/Core/Base.hpp"
#include "glash/Core/Timestep.hpp"


#include "glash/events/Event.hpp"

namespace Cine
{
	class GLASH_API Layer
	{
	public:
		Layer(const std::string& name = "Layer");
		
		virtual ~Layer() = default;

		virtual void OnAttach();
		virtual void OnDetach() {}
		virtual void OnFixedUpdate(Timestep fixedDeltaTime) {}
		virtual void OnUpdate(Timestep deltaTime) {}
		virtual void OnImGuiRender() {}
		virtual void OnEvent(Event& event) {}

		inline const std::string& GetName() const { return m_Name; }
		inline std::string ToString() const { return GetName();  }

	protected:
		std::string m_Name;
	};
}
STRING_FORMAT(Layer)
