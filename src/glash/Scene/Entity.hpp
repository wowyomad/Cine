#pragma once

#include <entt/entt.hpp>

#include "Scene.hpp"
#include "Components.hpp"

#include "glash/Math/Math.hpp"

namespace Cine
{
	struct HierarchyComponent;

	class Entity
	{
	public:
		Entity() = default;
		Entity(entt::entity handle, Scene* scene);
		Entity(const Entity& other) = default;

		bool IsValid() const
		{
			return m_Scene->m_Registry.valid(m_EntityHandle);
		}

		template <class Component>
		bool HasComponent()
		{
			return m_Scene->m_Registry.all_of<Component>(m_EntityHandle);
		}

		template <class... Components>
		bool HasComponents()
		{
			return m_Scene->m_Registry.all_of<Components...>(m_EntityHandle);
		}

		template <class Component, class... Args>
		Component& AddComponent(Args&&... args)
		{
			//CINE_CORE_ASSERT(!HasComponent<Component>(), "Entity already has component {}", typeid(Component).name());
			Component& component = m_Scene->m_Registry.emplace_or_replace<Component>(m_EntityHandle, std::forward<Args>(args)...);
			m_Scene->OnComponentAdded<Component>(*this, component);
			return component;
		}

		template <class... Components>
		std::tuple<Components&...>  AddComponents()
		{
			//CINE_CORE_ASSERT(!HasComponents<Components...>(), "Entity already has one or more components");
			std::tuple<Components&...> components = std::tuple<Components&...>(
				m_Scene->m_Registry.emplace_or_replace<Components>(m_EntityHandle)...
			);
			(m_Scene->OnComponentAdded<Components>(*this, std::get<Components&>(components)), ...);
			return components;
		}

		void AddComponentByName(const std::string& name)
		{
			m_Scene->AddComponentByName(*this, name);
		}

		template <class Component, class... Args>
		Component& AddOrReplaceComponent(Args&&... args)
		{
			Component& component = m_Scene->m_Registry.emplace_or_replace<Component>(m_EntityHandle, std::forward<Args>(args)...);

			return component;
		}

		template <class Component>
		void RemoveComponent()
		{
			m_Scene->OnComponentRemoved<Component>(*this);
		}

		template<class Component>
		Component& GetComponent()
		{
			CINE_CORE_ASSERT(HasComponent<Component>(), "Entity does not have component {}", typeid(Component).name());
			return m_Scene->m_Registry.get<Component>(m_EntityHandle);
		}
	

		template <class Component>
		Component* TryGetComponent()
		{
			if (m_EntityHandle != entt::null)
			{
				return m_Scene->m_Registry.try_get<Component>(m_EntityHandle);
			}
			return nullptr;
		}

		//Not really good for performance
		template<class... Components>
		auto GetComponents()
		{
			CINE_CORE_ASSERT(HasComponents<Components...>(), "Entity does not have all requested components");
			return m_Scene->m_Registry.get<Components...>(m_EntityHandle);
		}

		TransformComponent& Transform()
		{
			return GetComponent<TransformComponent>();
		}

		glm::vec3& LocalTranslation()
		{
			return Transform().Translation;
		}

		glm::vec3& LocalRotation()
		{
			return Transform().Rotation;
		}

		glm::vec3& LocalScale()
		{
			return Transform().Scale;
		}


		glm::vec3 Translation();
		glm::vec3 Rotation();
		glm::vec3 Scale();

		void OnTriggerEnter(Entity other);
		void OnTriggerExit(Entity other);

		UUID GetID() { return GetComponent<IDComponent>().ID; }


		Entity Clone();
		Entity GetParent();
		const std::vector<Entity>& GetChildren();
		void AddChild(Entity child);
		bool AddParent(Entity parent);
		bool AddParentWithoutTransform(Entity parent);
		void RemoveChild(Entity child);
		void RemoveChildren();
		void RemoveParent();

		void Destroy()
		{
			m_Scene->DestroyEntity(*this);
			m_EntityHandle = entt::null;
		}

		operator bool() const { return m_EntityHandle != entt::null && m_Scene; }
		operator entt::entity() const { return m_EntityHandle; }
		operator uint32_t() const { return static_cast<uint32_t>(m_EntityHandle); }
		bool operator==(const Entity& other) const { return m_EntityHandle == other.m_EntityHandle && m_Scene == other.m_Scene; }
		bool operator==(Entity&& other) const { return (*this) == other; }

		std::string ToString() const
		{
			if (*this)
			{
				return std::to_string(static_cast<uint32_t>(m_EntityHandle));
			}
			else
			{
				return "NullEntity";
			}
		}


	private:
		Scene* m_Scene = nullptr;
		entt::entity m_EntityHandle = entt::null;

		friend class Scene;
		friend class NativeScript;
	};

	struct HierarchyComponent
	{
		Entity Parent;
		std::vector<Entity> Children;
	};
}