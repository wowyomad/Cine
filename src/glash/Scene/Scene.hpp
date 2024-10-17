#pragma once
#include "glash/Core/Base.hpp"
#include "glash/Core/Timestep.hpp"
#include "glash/Renderer/EditorCamera.hpp"

#include "glash/Scene/Components.hpp"
#include <entt/entt.hpp>


namespace Cine
{
	class Entity;
	class ScriptableEntity;

	class Scene
	{
	public:
		Scene();
		~Scene();

		Entity CreateEntity(const std::string& name = std::string());
		void DestroyEntity(Entity entity);

		void SetMainCamera(Entity cameraEntity);
		Entity GetMainCameraEntity();

		void OnViewportResize(uint32_t width, uint32_t height);

		void OnUpdateRuntime(Timestep ts);
		void OnUpdateEditor(Timestep ts, EditorCamera& editorCamera);

	private:

		template <class Component>
		void OnComponentAdded(Entity entity, Component& component)
		{
			if constexpr (std::is_same<Component, CameraComponent>::value)
			{
				if (m_Registry.all_of<NativeScriptComponent>(entity))
				{
					CINE_ASSERT(false, "xyu");
				}
				component.Camera.SetViewportSize(m_ViewportWidth, m_ViewportHeight);
				CINE_CORE_TRACE("Added Camera Component to {}", m_Registry.get<TagComponent>(entity).Tag);
			}

			if constexpr (std::is_base_of<ScriptableEntity, Component>::value)
			{
				bool hasNativeScriptComponent = m_Registry.all_of<NativeScriptComponent>(entity);
				if (!hasNativeScriptComponent)
				{
					CINE_CORE_TRACE("NativeScripteComponent added to {}", static_cast<unsigned int>(entity));
					m_Registry.emplace<NativeScriptComponent>(entity);
				}
				CINE_CORE_TRACE("Script added to {}", static_cast<unsigned int>(entity));
				auto& nsc = m_Registry.get<NativeScriptComponent>(entity);
				nsc.Bind<Component>();

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