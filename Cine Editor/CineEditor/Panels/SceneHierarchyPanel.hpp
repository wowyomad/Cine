#pragma once
#include "glash/glash_pch.hpp"

#include "glash/Core/Base.hpp"
#include "glash/Scene/Scene.hpp"
#include "glash/Scene/Entity.hpp"
#include "glash/Scene/SceneCamera.hpp"
#include "glash/Scene/Components.hpp"

#include <imgui/ImGuiLayer.hpp>

namespace Cine
{
	class SceneHierarchyPanel
	{
		struct Context
		{
			Ref<Scene> Scene;
			Entity Selection;
			Entity Properties;
		};

	public:
		SceneHierarchyPanel() = default;
		SceneHierarchyPanel(const Ref<Scene>& scene);

		void DeleteSelectedEntity();

		void SetContext(const Ref<Scene>& scene);
		void SetSelectedEntity(Entity entity);
		void SetPropertiesEntity(Entity entity);

		void OnImGuiRender();

		Entity GetSelectedEntity() const;

	private:
		Context m_Context;


	private:
		void DisplayEntityNode(Entity entity);
		void DisplayEntityProperties(Entity entity);

		void DisplayTransformComponent(Entity entity);
		void DisplayTagComponent(Entity entity);
		void DisplaySpriteRendererComponent(Entity entity);
		void DisplaySpriteSheetComponent(Entity entity);
		void DisplaySpriteComponent(Entity entity);
		void DisplayCameraComponent(Entity entity);
		void DisplayRigidBody2DComponent(Entity entity);
		void DisplayBoxCollider2DComponent(Entity entity);
		void DisplayNativeScriptComponent(Entity entity);

		void AddScriptPopup(Entity entity);


	};
}