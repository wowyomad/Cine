#pragma once
#include "glash/Core/Base.hpp"
#include "glash/Core/Timestep.hpp"

#include "glash/Scene/Components.hpp"

#include <entt/entt.hpp>

namespace Cine
{
	class Entity;

	class Scene
	{
	public:
		Scene();
		~Scene();

		Entity CreateEntity(const std::string& name = std::string());
		void DestroyEntity(Entity entity);

		void SetMainCamera(Entity cameraEntity);
		Entity GetMainCamera();

		void OnViewportResize(uint32_t width, uint32_t height);
		
		void OnUpdate(Timestep ts);

	private:
		template <class Component>
		void OnComponentAdded(Entity entity, Component& component)
		{
			if constexpr (std::is_same<Component, CameraComponent>::value)
			{
				component.Camera.SetViewportSize(m_ViewportWidth, m_ViewportHeight);
				CINE_CORE_TRACE("Added Camera Component to {}", m_Registry.get<TagComponent>(entity).Tag);
			}
		}
	

	private:
		entt::registry m_Registry;
		Entity* m_MainCamera;

		uint32_t m_ViewportWidth = 1;
		uint32_t m_ViewportHeight = 1;

		friend class Entity;
		friend class SceneHierarchyPanel;
		friend class SceneSerializer;


	};
}