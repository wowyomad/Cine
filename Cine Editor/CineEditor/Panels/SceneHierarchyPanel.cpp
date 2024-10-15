#include "SceneHierarchyPanel.hpp"

#include "glash/Scene/Components.hpp"

#include <imgui_internal.h>

namespace Cine
{
	static void DisplayVec3Control(const std::string& label, glm::vec3& values, float resetValue = 0.0f, float columnWidth = 100.0f)
	{
		ImGui::PushID(label.c_str());

		ImGui::Columns(2);
		ImGui::SetColumnWidth(0, columnWidth);
		ImGui::Text(label.c_str());
		ImGui::NextColumn();

		ImGui::PushMultiItemsWidths(3, ImGui::CalcItemWidth());
		ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, { 0.0f, 0.0f });

		float lineHeight = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.0f;
		ImVec2 buttonSize = { lineHeight + 3.0f, lineHeight };

		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.8f, 0.1f, 0.15f, 1.0f));
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.9f, 0.2f, 0.12f, 1.0f));
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.8f, 0.1f, 0.15f, 1.0f));
		if (ImGui::Button("X", buttonSize))
		{
			values.x = resetValue;
		}
		ImGui::PopStyleColor(3);
		ImGui::SameLine();
		ImGui::DragFloat("##X", &values.x, 0.1f, 0.0f, 0.0f, "%.2f");
		ImGui::PopItemWidth();
		ImGui::SameLine();

		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.2f, 0.7f, 0.3f, 1.0f));
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.3f, 0.8f, 0.3f, 1.0f));
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.2f, 0.7f, 0.2f, 1.0f));
		if (ImGui::Button("Y", buttonSize))
		{
			values.y = resetValue;
		}
		ImGui::PopStyleColor(3);

		ImGui::SameLine();
		ImGui::DragFloat("##Y", &values.y, 0.1f, 0.0f, 0.0f, "%.2f");
		ImGui::PopItemWidth();
		ImGui::SameLine();

		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.1f, 0.25f, 0.8f, 1.0f));
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.2f, 0.35f, 0.9f, 1.0f));
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.1f, 0.25f, 0.8f, 1.0f));
		if (ImGui::Button("Z", buttonSize))
		{
			values.z = resetValue;
		}
		ImGui::PopStyleColor(3);

		ImGui::SameLine();
		ImGui::DragFloat("##Z", &values.z, 0.1f, 0.0f, 0.0f, "%.2f");
		ImGui::PopItemWidth();
		ImGui::SameLine();

		ImGui::PopStyleVar();

		ImGui::Columns(1);
		ImGui::PopID();
	}

	SceneHierarchyPanel::SceneHierarchyPanel(const Ref<Scene>& scene)
	{
		SetContext(scene);
	}

	void SceneHierarchyPanel::SetContext(const Ref<Scene>& scene)
	{
		m_Context.Scene = scene;
	}

	void SceneHierarchyPanel::OnImGuiRender()
	{
		ImGui::Begin("Scene Hierarchy");
		{
			Scene& scene = *m_Context.Scene.get();
			auto view = scene.m_Registry.view<entt::entity>();
			for (auto entityID : view)
			{
				Entity entity(entityID, &scene);
				DisplayEntityNode(entity);
			}
		}

		if (ImGui::BeginPopupContextWindow(0, ImGuiPopupFlags_MouseButtonDefault_ | ImGuiPopupFlags_NoOpenOverItems))
		{
			if (ImGui::MenuItem("Create Empty Entity"))
			{
				m_Context.Scene->CreateEntity("Empty Entity");
			}
			ImGui::EndPopup();
		}

		ImGui::End();




		ImGui::Begin("Properties");
		if (m_Context.Properties)
		{
			DisplayEntityProperties(m_Context.Properties);

			if (ImGui::Button("Add Component"))
			{
				ImGui::OpenPopup("AddComponent");
			}
			if (ImGui::BeginPopup("AddComponent"))
			{
				if (ImGui::MenuItem("Camera"))
				{
					m_Context.Properties.AddComponent<CameraComponent>();
					ImGui::CloseCurrentPopup();
				}

				if (ImGui::MenuItem("Sprite Renderer"))
				{
					m_Context.Properties.AddComponent<SpriteRendererComponent>();
					ImGui::CloseCurrentPopup();
				}
				ImGui::EndPopup();
			}

		}
		ImGui::End();

	}

	void SceneHierarchyPanel::DisplayEntityNode(Entity entity)
	{
		auto& tag = m_Context.Scene->m_Registry.get<TagComponent>(entity).Tag;

		ImGuiTreeNodeFlags flags = ((m_Context.Selection == entity) ? ImGuiTreeNodeFlags_OpenOnArrow : 0);
		if (m_Context.Selection == entity)
		{
			flags |= ImGuiTreeNodeFlags_Selected;
		}

		bool expanded = ImGui::TreeNodeEx((const void*)(uint64_t)(uint32_t)entity, flags, tag.c_str());

		if (ImGui::IsItemClicked() && !ImGui::IsMouseDoubleClicked(0))
		{
			m_Context.Selection = entity;
		}
		else if (ImGui::IsMouseDoubleClicked(0) && ImGui::IsItemHovered())
		{
			m_Context.Selection = entity;
			m_Context.Properties = entity;
		}

		bool entityDeleted = false;
		if (ImGui::BeginPopupContextItem())
		{
			if (ImGui::MenuItem("Delete Entity"))
			{
				entityDeleted = true;
			}
			ImGui::EndPopup();
		}


		if (expanded)
		{
			ImGui::TreePop();
		}

		if (entityDeleted)
		{
			m_Context.Scene->DestroyEntity(entity);
			if (m_Context.Selection == entity)
			{
				m_Context.Selection = { };
			}
			if (m_Context.Properties == entity)
			{
				m_Context.Properties = { };
			}
		}
	}

	void SceneHierarchyPanel::DisplayEntityProperties(Entity entity)
	{
		if (entity.HasComponent<TagComponent>())
		{
			ImGui::Spacing();
			ImGui::Separator();
			ImGui::Spacing();
			DisplayTagComponentProperty(entity);
		}

		if (entity.HasComponent<TransformComponent>())
		{
			ImGui::Spacing();
			ImGui::Separator();
			ImGui::Spacing();
			DisplayTransformComponentProperty(entity);
		}

		if (entity.HasComponent<CameraComponent>())
		{
			ImGui::Spacing();
			ImGui::Separator();
			ImGui::Spacing();
			DisplayCameraComponentProperty(entity);
		}

		if (entity.HasComponent<SpriteRendererComponent>())
		{
			ImGui::Spacing();
			ImGui::Separator();
			ImGui::Spacing();
			DisplaySpriteRendererProperty(entity);
		}
	}

	void SceneHierarchyPanel::DisplayTransformComponentProperty(Entity entity)
	{
		DisplayComponentProperty<TransformComponent>(entity, "Transform", [](TransformComponent& tc)
			{
				auto& translation = tc.Translation;
				DisplayVec3Control("Position", translation);

				auto rotation = glm::degrees(tc.Rotation);
				DisplayVec3Control("Rotation", rotation);
				tc.Rotation = glm::radians(rotation);

				auto& scale = tc.Scale;
				DisplayVec3Control("Scale", scale, 1.0f);


			});
	}

	void SceneHierarchyPanel::DisplayTagComponentProperty(Entity entity)
	{
		DisplayComponentProperty<TagComponent>(entity, "Tag", [](auto& tagComponent)
			{
				auto& tag = tagComponent.Tag;
				char buffer[256]{ 0 };
				strncpy(buffer, tag.c_str(), sizeof(buffer));
				if (ImGui::InputText("Tag", buffer, sizeof(buffer)))
				{
					tag = std::string(buffer);
				}
			});
	}

	void SceneHierarchyPanel::DisplaySpriteRendererProperty(Entity entity)
	{
		DisplayComponentProperty<SpriteRendererComponent>(entity, "Sprite Renderer", [](SpriteRendererComponent& spriteRendererComponent)
			{
				auto& color = spriteRendererComponent.Color;
				ImGui::ColorEdit4("Color", glm::value_ptr(color));
			});
	}

	void SceneHierarchyPanel::DisplayCameraComponentProperty(Entity entity)
	{
		using ProjectionType = SceneCamera::ProjectionType;
		DisplayComponentProperty<CameraComponent>(entity, "Camera", [this, entity](auto& cameraComponent)
			{
				SceneCamera& camera = cameraComponent.Camera;
				constexpr std::array<const char*, 2> projectionTypeStrings = { "Perspective", "Orthographic" };
				ProjectionType currentProjectionType = static_cast<ProjectionType>(camera.GetProjectionType());
				const char* currentProjectionTypeString = projectionTypeStrings[static_cast<int>(currentProjectionType)];

				if (ImGui::BeginCombo("Projection", currentProjectionTypeString))
				{
					for (size_t i = 0; i < projectionTypeStrings.size(); ++i)
					{
						bool isSelected = (currentProjectionType == static_cast<ProjectionType>(i));
						if (ImGui::Selectable(projectionTypeStrings[i], isSelected))
						{
							currentProjectionType = static_cast<ProjectionType>(i);
							camera.SetProjectionType(static_cast<SceneCamera::ProjectionType>(i));
						}
						if (isSelected)
						{
							ImGui::SetItemDefaultFocus();
						}
					}
					ImGui::EndCombo();
				}

				bool isMainCamera = entity == m_Context.Scene->GetMainCamera();
				if (ImGui::Checkbox("Main Camera", &isMainCamera))
				{
					m_Context.Scene->SetMainCamera(entity);
				}

				switch (currentProjectionType)
				{
				case ProjectionType::Orthographic:
					DisplayOrthographicCameraProperty(camera);
					break;
				case ProjectionType::Perspective:
					DisplayPerspectiveCameraProperty(camera);
					break;
				}
			});
	}

	void SceneHierarchyPanel::DisplayOrthographicCameraProperty(SceneCamera& camera)
	{
		float orthoSize = camera.GetOrthographicSize();
		float nearClip = camera.GetOrthographicNearClip();
		float farClip = camera.GetOrthographicFarClip();

		if (ImGui::DragFloat("Size", &orthoSize, 0.1f, 0.01f, 1000.0f, "%.3f")) {
			camera.SetOrthographicSize(orthoSize);
		}

		if (ImGui::DragFloat("Near", &nearClip, 0.1f, -1000.0f, farClip, "%.3f")) {
			camera.SetOrthographicNearClip(nearClip);
		}

		if (ImGui::DragFloat("Far", &farClip, 0.1f, nearClip, 1000.0f, "%.3f")) {
			camera.SetOrthographicFarClip(farClip);
		}
	}

	void SceneHierarchyPanel::DisplayPerspectiveCameraProperty(SceneCamera& camera)
	{
		float verticalFOV = glm::degrees(camera.GetPerspectiveVerticalFOV());
		float nearClip = camera.GetPerspectiveNearClip();
		float farClip = camera.GetPerspectiveFarClip();

		if (ImGui::DragFloat("Vertical FOV", &verticalFOV, 0.1f, 0.01f, 360.0f, "%.3f")) {
			camera.SetPerspectiveVerticalFOV(glm::radians(verticalFOV));
		}

		if (ImGui::DragFloat("Near", &nearClip, 0.1f, -1000.0f, farClip, "%.3f")) {
			camera.SetPerspectiveNearClip(nearClip);
		}

		if (ImGui::DragFloat("Far", &farClip, 0.1f, nearClip, 1000.0f, "%.3f")) {
			camera.SetPerspectiveFarClip(farClip);
		}
	}

}