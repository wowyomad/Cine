#pragma once

#include "Entity.hpp"
#include "glash/Core/Timestep.hpp"

namespace Cine
{
	class ScriptableEntity
	{
	public:
		ScriptableEntity() = default;
		virtual ~ScriptableEntity() {}

		template <class T>
		T& GetComponent()
		{
			return m_Entity.GetComponent<T>();
		}

		template <class T>
		bool HasComponent()
		{
			return m_Entity.HasComponent<T>();
		}

		const Entity GetEntity() const { return m_Entity; }

	public:
		virtual void OnCreate() {}
		virtual void OnDestroy() {}
		virtual void OnUpdate(Timestep ts) {}

	public:
		Entity m_Entity;
	};
}
