#pragma once

#include <entt/entt.hpp>

#include "Scene.hpp"

namespace Cine
{
	struct HierarchyComponent;

	class Entity
	{
	public:
		Entity() = default;
		Entity(entt::entity handle, Scene* scene);
		Entity(const Entity& other) = default;

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
			CINE_CORE_ASSERT(!HasComponent<Component>(), "Entity already has component {}", typeid(Component).name());
			Component& component = m_Scene->m_Registry.emplace<Component>(m_EntityHandle, std::forward<Args>(args)...);
			m_Scene->OnRegisteredComponentAdded<Component>(*this, component);
			return component;
		}

		template <class... Components>
		std::tuple<Components&...>  AddComponents()
		{
			CINE_CORE_ASSERT(!HasComponents<Components...>(), "Entity already has one or more components");
			std::tuple<Components&...> components = std::tuple<Components&...>(
				m_Scene->m_Registry.emplace<Components>(m_EntityHandle)...
			);
			(m_Scene->OnRegisteredComponentAdded<Components>(*this, std::get<Components&>(components)), ...);
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

		template<class Component>
		Component& GetComponent()
		{
			CINE_CORE_ASSERT(HasComponent<Component>(), "Entity does not have component {}", typeid(Component).name());
			return m_Scene->m_Registry.get<Component>(m_EntityHandle);
		}

		template <class Component>
		Component* TryGetComponent()
		{
			return m_Scene->m_Registry.try_get<Component>(m_EntityHandle);
		}

		//Not really good for performance
		template<class... Components>
		auto GetComponents()
		{
			CINE_CORE_ASSERT(HasComponents<Components...>(), "Entity does not have all requested components");
			return m_Scene->m_Registry.get<Components...>(m_EntityHandle);
		}



		template <class Component>
		void RemoveComponent()
		{
			CINE_CORE_WARN("Removing component {0}", Utils::GetClassTypename<Component>());
			CINE_CORE_ASSERT(HasComponent<Component>(), "Entity does not have component {}", typeid(Component).name());
			m_Scene->m_Registry.remove<Component>(m_EntityHandle);
		}

		Entity GetParent();
		const std::vector<Entity>& GetChildren();
		void AddChild(Entity child);
		bool AddParent(Entity parent);
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
	};

	struct HierarchyComponent
	{
		Entity Parent;
		std::vector<Entity> Children;
	};
}