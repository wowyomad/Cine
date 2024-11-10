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
			return Object.TryGetComponent<T>();
		}

		template <class T>
		T& GetComponent()
		{
			return Object.GetComponent<T>();
		}

		template <class T>
		bool HasComponent()
		{
			return Object.HasComponent<T>();
		}

		const Entity GetEntity() const { return Object; }

	public:
		virtual void OnCreate() {}
		virtual void OnDestroy() {}
		virtual void OnUpdate(Timestep ts) {}

		virtual void OnTriggerEnter(Entity other) { }
		virtual void OnTriggerExit(Entity other) { }

		Entity CreateEntity()
		{
			return Object.m_Scene->CreateEntity();
		}

		Entity CreateEntity(Entity reference)
		{
			return reference.Clone();
		}

		TransformComponent& Transform() { return Object.Transform(); }

		glm::vec3& LocalTranslation()
		{
			return Object.LocalTranslation();
		}
		glm::vec3& LocalRotation()
		{
			return Object.LocalRotation();
		}
		glm::vec3& LocalScale()
		{
			return Object.LocalRotation();
		}

		glm::vec3 Translation()
		{
			return Object.Translation();
		}
		glm::vec3 Rotation()
		{
			return Object.Rotation();
		}
		glm::vec3 Scale()
		{
			return Object.Scale();
		}


	public:
		Entity Object;



	public:
		bool Enabled = true;
	};
}
