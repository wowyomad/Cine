#pragma once

#include "Entity.hpp"
#include "Scene.hpp"

#include "glash/Core/Timestep.hpp"

namespace Cine
{
	class ScriptableEntity
	{
	public:
		virtual ~ScriptableEntity() {}

		template <class T>
		T& GetComponent()
		{
			return m_Entity.GetComponent<T>();
		}

		const Entity GetEntity() const { return m_Entity; }

	protected:
		virtual void OnCreate() {}
		virtual void OnDestroy() {}
		virtual void OnUpdate(Timestep ts) {}

	private:
		Entity m_Entity;
		friend class Scene;
	};
}