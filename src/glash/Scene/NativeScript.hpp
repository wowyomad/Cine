#pragma once

#include "Entity.hpp"
#include "glash/Core/Timestep.hpp"

namespace Cine
{
	class NativeScript
	{
		friend class Scene;
		friend class Entity;

	public:
		NativeScript() = default;
		NativeScript(const NativeScript&) = default;
		NativeScript(NativeScript&&) = default;

		NativeScript& operator=(const NativeScript&) = default;

		virtual ~NativeScript() { }

		template <class T>
		T* TryGetComponent()
		{
			return m_Entity.TryGetComponent<T>();
		}

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

		virtual void OnTriggerEnter(Entity other) { }
		virtual void OnTriggerExit(Entity other) { }


		Entity CreateEntity()
		{
			return m_Entity.m_Scene->CreateEntity();
		}

		Entity CreateEntity(Entity reference)
		{
			return reference.Clone();
		}


	private:
		Entity m_Entity;



	public:
		bool Enabled = true;
	};
}
