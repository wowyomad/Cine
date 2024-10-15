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

		void SetContext(const Ref<Scene>& scene);

		void OnImGuiRender();

	private:
		Context m_Context;


	private:
		void DisplayEntityNode(Entity entity);
		void DisplayEntityProperties(Entity entity);

		void DisplayTransformComponentProperty(Entity entity);
		
		void DisplayTagComponentProperty(Entity entity);

		void DisplaySpriteRendererProperty(Entity entity);

		void DisplayCameraComponentProperty(Entity entity);
		void DisplayOrthographicCameraProperty(SceneCamera& camera);
		void DisplayPerspectiveCameraProperty(SceneCamera& camera);

		template<typename ComponentType>
		struct IsNonRemovableComponent
		{
			static constexpr bool value =
				std::is_same<ComponentType, TransformComponent>::value ||
				std::is_same<ComponentType, TagComponent>::value;
		};


		template<typename ComponentType, typename Func>
		void DisplayComponentProperty(Entity entity, const char* name, Func&& func)
		{
			constexpr ImGuiTreeNodeFlags treeNodeFlags = ImGuiTreeNodeFlags_DefaultOpen;
			bool removeComponent = false;

			
			ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, { 4.0f, 4.0f });
			bool open = ImGui::TreeNodeEx((void*)typeid(ComponentType).hash_code(), treeNodeFlags, name);

			if constexpr (!IsNonRemovableComponent<ComponentType>::value)
			{
				ImGui::SameLine(ImGui::GetWindowWidth() - 30.0f);
				if (ImGui::Button("...", { 20.0f, 20.0f }))
				{
					ImGui::OpenPopup("ComponentSettings");
				}
				ImGui::PopStyleVar();
				if (ImGui::BeginPopup("ComponentSettings"))
				{
					if (ImGui::MenuItem("Remove Component"))
					{
						removeComponent = true;
					}
					ImGui::EndPopup();
				}
			}
			if constexpr (IsNonRemovableComponent<ComponentType>::value)
			{
				ImGui::PopStyleVar();
			}


			


			if (open)
			{
				func(entity.GetComponent<ComponentType>());
				ImGui::TreePop();
			}

			if (removeComponent)
			{
				entity.RemoveComponent<ComponentType>();
			}
		}
	};
}