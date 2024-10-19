#include "glash/glash_pch.hpp"
#include "SceneHierarchyPanel.hpp"
#include "glash/Cine.hpp"

#include "glash/Scene/Components.hpp"
#include "glash/Scene/SceneSerializer.hpp"
#include "glash/Scene/AssetManager.hpp"

#include <imgui_internal.h>

namespace Cine
{
	template<typename ComponentType>
	struct IsNonRemovableComponent
	{
		static constexpr bool value =
			std::is_same<ComponentType, TransformComponent>::value ||
			std::is_same<ComponentType, TagComponent>::value;
	};

	template<class Component>
	void AddComponentItem(Entity entity, const char* name)
	{
		bool hasComponent = entity.HasComponent<Component>();
		if (hasComponent)
			ImGui::BeginDisabled();

		if (ImGui::MenuItem(name))
		{
			entity.AddComponent<Component>();
			if constexpr (std::is_base_of<SpriteRendererComponent, Component>::value)
			{
				if (!entity.HasComponent<SpriteComponent>())
				{
					entity.AddComponent<SpriteComponent>();
				}
				if (!entity.HasComponent<SpriteSheetComponent>())
				{
					entity.AddComponent<SpriteSheetComponent>();
				}
			}
			ImGui::CloseCurrentPopup();
		}
		if (hasComponent)
			ImGui::EndDisabled();
	};

	template<typename T, typename Func>
	void DisplayComponent(Entity entity, const char* name, Func&& displayFunction)
	{
		ImGui::Spacing();

		constexpr ImGuiTreeNodeFlags treeNodeFlags = ImGuiTreeNodeFlags_DefaultOpen
			| ImGuiTreeNodeFlags_AllowItemOverlap
			| ImGuiTreeNodeFlags_Framed
			| ImGuiTreeNodeFlags_FramePadding
			| ImGuiTreeNodeFlags_SpanAvailWidth
			;

		ImVec2 contentRegionAvailable = ImGui::GetContentRegionAvail();

		ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, { 4.0f, 4.0f });
		float lineHeigh = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.0f;
		ImGui::Separator();
		
		bool open = ImGui::TreeNodeEx((void*)typeid(T).hash_code(), treeNodeFlags, name);
		ImGui::PopStyleVar();

		bool removeComponent = false;
		if constexpr (!IsNonRemovableComponent<T>::value)
		{
			ImGui::SameLine(contentRegionAvailable.x - lineHeigh * 0.5f);
			if (ImGui::Button("+", { lineHeigh, lineHeigh }))
			{
				ImGui::OpenPopup("ComponentSettings");
			}
			if (ImGui::BeginPopup("ComponentSettings"))
			{
				if (ImGui::MenuItem("Remove Component"))
				{
					removeComponent = true;
				}
				ImGui::EndPopup();
			}
		}

		if (open)
		{
			displayFunction(entity.GetComponent<T>());
			ImGui::TreePop();
		}

		if (removeComponent)
		{
			entity.RemoveComponent<T>();
		}
	}

	static void DisplayVec3Control(const std::string& label, glm::vec3& values, float resetValue = 0.0f, float columnWidth = 100.0f)
	{
		ImGuiIO& io = ImGui::GetIO();
		auto boldFont = io.Fonts->Fonts[1];

		ImGui::PushID(label.c_str());

		ImGui::Columns(2);
		ImGui::SetColumnWidth(0, columnWidth);
		ImGui::Text(label.c_str());
		ImGui::NextColumn();

		ImGui::PushMultiItemsWidths(3, ImGui::CalcItemWidth());
		ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, { 0.0f, 0.0f });

		float lineHeight = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.0f;
		ImVec2 buttonSize = { lineHeight + 3.0f, lineHeight };

		auto drawControl = [&](const char* label, float& value, const ImVec4& color) {
			ImGui::PushStyleColor(ImGuiCol_Button, color);
			ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(color.x + 0.1f, color.y + 0.1f, color.z + 0.1f, 1.0f));
			ImGui::PushStyleColor(ImGuiCol_ButtonActive, color);
			ImGui::PushFont(boldFont);

			if (ImGui::Button(label, buttonSize))
				value = resetValue;

			ImGui::PopFont();
			ImGui::PopStyleColor(3);
			ImGui::SameLine();
			ImGui::DragFloat(("##" + std::string(label)).c_str(), &value, 0.1f, 0.0f, 0.0f, "%.2f");
			ImGui::PopItemWidth();
			ImGui::SameLine();
			};

		drawControl("X", values.x, ImVec4{ 0.8f, 0.1f, 0.15f, 1.0f });
		drawControl("Y", values.y, ImVec4{ 0.2f, 0.7f, 0.3f, 1.0f });
		drawControl("Z", values.z, ImVec4{ 0.1f, 0.25f, 0.8f, 1.0f });

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
		m_Context.Selection = {};
		m_Context.Properties = {};
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

		if (ImGui::IsMouseDoubleClicked(0) && ImGui::IsWindowHovered() && !ImGui::IsAnyItemHovered())
		{
			m_Context.Selection = {};
			m_Context.Properties = {};
		}

		if (ImGui::BeginPopupContextWindow(0, ImGuiPopupFlags_MouseButtonDefault_ | ImGuiPopupFlags_NoOpenOverItems))
		{
			if (ImGui::MenuItem("Create Empty Entity"))
			{
				Entity newEntity = m_Context.Scene->CreateEntity("Empty Entity");
				m_Context.Properties = newEntity;
			}
			ImGui::EndPopup();
		}

		ImGui::End();


		ImGui::Begin("Properties");
		if (m_Context.Properties)
		{
			DisplayEntityProperties(m_Context.Properties);

			ImGui::Dummy(ImVec2(0.0f, 10.0f));

			float buttonWidth = ImGui::CalcTextSize("Add Component").x + ImGui::GetStyle().FramePadding.x * 2.0f;
			float availableWidth = ImGui::GetContentRegionAvail().x;
			float buttonPosX = (availableWidth - buttonWidth) * 0.5f;
			ImGui::SetCursorPosX(ImGui::GetCursorPosX() + buttonPosX);


			if (ImGui::Button("Add Component"))
			{
				ImGui::OpenPopup("AddComponent");
			}
			if (ImGui::BeginPopup("AddComponent"))
			{
				AddComponentItem<CameraComponent>(m_Context.Properties, "Camera");
				AddComponentItem<SpriteRendererComponent>(m_Context.Properties, "Sprite Renderer");
				ImGui::EndPopup();
			}

		}
		ImGui::End();
	}

	Entity SceneHierarchyPanel::GetSelectedEntity() const
	{
		return m_Context.Selection;
	}

	void SceneHierarchyPanel::DisplayEntityNode(Entity entity)
	{
		auto& tag = m_Context.Scene->m_Registry.get<TagComponent>(entity).Tag;

		ImGuiTreeNodeFlags flags = ((m_Context.Selection == entity) ? ImGuiTreeNodeFlags_OpenOnArrow : 0);
		flags |= ImGuiTreeNodeFlags_SpanAvailWidth;
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
			ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_SpanAvailWidth;
			bool opened = ImGui::TreeNodeEx((void*)9817239, flags, tag.c_str());
			if (opened)
			{
				ImGui::TreePop();
			}
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
			DisplayTagComponent(entity);
		}

		if (entity.HasComponent<TransformComponent>())
		{
			DisplayTransformComponent(entity);
		}

		if (entity.HasComponent<CameraComponent>())
		{
			DisplayCameraComponent(entity);
		}

		if (entity.HasComponent<SpriteRendererComponent>())
		{
			DisplaySpriteRendererComponent(entity);
		}

		if (entity.HasComponent<SpriteSheetComponent>())
		{
			DisplaySpriteSheetComponent(entity);
		}

		if (entity.HasComponent<SpriteComponent>())
		{
			DisplaySpriteComponent(entity);
		}
	}

	void SceneHierarchyPanel::DisplayTransformComponent(Entity entity)
	{
		ImGui::IsItemDeactivated();
		DisplayComponent<TransformComponent>(entity, "Transform", [](TransformComponent& tc)
			{
				auto& translation = tc.Translation;
				DisplayVec3Control("Position", translation);

				ImGui::Spacing();

				auto rotation = glm::degrees(tc.Rotation);
				DisplayVec3Control("Rotation", rotation);
				tc.Rotation = glm::radians(rotation);

				ImGui::Spacing();

				auto& scale = tc.Scale;
				DisplayVec3Control("Scale", scale, 1.0f);
			});
	}

	void SceneHierarchyPanel::DisplayTagComponent(Entity entity)
	{
		DisplayComponent<TagComponent>(entity, "Tag", [](auto& tagComponent)
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

	void SceneHierarchyPanel::DisplaySpriteComponent(Entity entity)
	{
		DisplayComponent<SpriteComponent>(entity, "Sprite", [&entity](SpriteComponent& sprite)
			{
				ImGui::ColorEdit4("Color", glm::value_ptr(sprite.Color));
				if (entity.HasComponent<SpriteSheetComponent>())
				{
					int32_t maxIndex = entity.GetComponent<SpriteSheetComponent>().Frames.size() - 1;
					ImGui::SliderInt("Sprite Sheet Index", &sprite.SpriteIndex, 0, maxIndex);
				}
			});

	}


	void SceneHierarchyPanel::DisplaySpriteSheetComponent(Entity entity)
	{
		DisplayComponent<SpriteSheetComponent>(entity, "Sprite Sheet", [&entity](SpriteSheetComponent& sheet)
			{
				if (sheet.Texture)
				{
					constexpr float step = 32.0f;
					float texWidth = static_cast<float>(sheet.Texture->GetWidth());
					float texHeight = static_cast<float>(sheet.Texture->GetHeight());

					for (size_t i = 0; i < sheet.Frames.size(); ++i) {
						auto& frame = sheet.Frames[i];
						ImGui::PushID(static_cast<int>(i));
						ImGui::Text("Frame %d", static_cast<int>(i));

						int x0 = static_cast<int>(frame.Coords[0].x * texWidth);
						int y0 = static_cast<int>(frame.Coords[0].y * texHeight);
						int x1 = static_cast<int>(frame.Coords[1].x * texWidth);
						int y1 = static_cast<int>(frame.Coords[1].y * texHeight);
						int x2 = static_cast<int>(frame.Coords[2].x * texWidth);
						int y2 = static_cast<int>(frame.Coords[2].y * texHeight);
						int x3 = static_cast<int>(frame.Coords[3].x * texWidth);
						int y3 = static_cast<int>(frame.Coords[3].y * texHeight);

						ImGui::SetNextItemWidth(80);
						ImGui::DragInt("u0", &x0, step, 0, static_cast<int>(texWidth));
						ImGui::SameLine();
						ImGui::SetNextItemWidth(80);
						ImGui::DragInt("v0", &y0, step, 0, static_cast<int>(texHeight));

						ImGui::SetNextItemWidth(80);
						ImGui::DragInt("u1", &x1, step, 0, static_cast<int>(texWidth));
						ImGui::SameLine();
						ImGui::SetNextItemWidth(80);
						ImGui::DragInt("v1", &y1, step, 0, static_cast<int>(texHeight));

						ImGui::SetNextItemWidth(80);
						ImGui::DragInt("u2", &x2, step, 0, static_cast<int>(texWidth));
						ImGui::SameLine();
						ImGui::SetNextItemWidth(80);
						ImGui::DragInt("v2", &y2, step, 0, static_cast<int>(texHeight));

						ImGui::SetNextItemWidth(80);
						ImGui::DragInt("u3", &x3, step, 0, static_cast<int>(texWidth));
						ImGui::SameLine();
						ImGui::SetNextItemWidth(80);
						ImGui::DragInt("v3", &y3, step, 0, static_cast<int>(texHeight));

						frame.Coords[0].x = static_cast<float>(x0) / texWidth;
						frame.Coords[0].y = static_cast<float>(y0) / texHeight;
						frame.Coords[1].x = static_cast<float>(x1) / texWidth;
						frame.Coords[1].y = static_cast<float>(y1) / texHeight;
						frame.Coords[2].x = static_cast<float>(x2) / texWidth;
						frame.Coords[2].y = static_cast<float>(y2) / texHeight;
						frame.Coords[3].x = static_cast<float>(x3) / texWidth;
						frame.Coords[3].y = static_cast<float>(y3) / texHeight;

						float contentWidth = ImGui::GetContentRegionAvail().x;
						ImGui::SetCursorPosX(contentWidth - 50);
						ImGui::SetNextItemWidth(90);
						if (ImGui::Button("Remove")) {
							sheet.Frames.erase(sheet.Frames.begin() + i);
							--i;
						}

						ImGui::PopID();
						ImGui::Separator();
					}

					if (ImGui::Button("Add New Frame")) {
						float defaultX = 0.0f;
						float defaultY = 0.0f;
						float defaultWidth = 64.0f / texWidth;
						float defaultHeight = 64.0f / texHeight;

						SpriteSheetComponent::Frame newFrame = {
							{ { { defaultX, defaultY }, { defaultX + defaultWidth, defaultY },
								{ defaultX + defaultWidth, defaultY + defaultHeight }, { defaultX, defaultY + defaultHeight } } }
						};
						sheet.Frames.push_back(newFrame);
					}
				}
				
				ImGui::Separator();
				ImGui::BeginDisabled();
				if (sheet.Texture && ImGui::Button("Save SpriteSheet"))
				{
					std::filesystem::path filepath = FileDialogs::SaveFile("Sprite Sheet (*.png)\0*.png\0");
					if (!filepath.empty())
					{
						//TODO: Save Sprite Sheet

					}
				}
				ImGui::EndDisabled();

				ImGui::SameLine();
				if (ImGui::Button("Load SpriteSheet"))
				{
					std::filesystem::path filepath = FileDialogs::OpenFile("Sprite Sheet (*.png)\0*.png\0");
					if (!filepath.empty())
					{
						sheet = AssetManager::LoadSpriteSheet("dummy", filepath); //TODO: SpriteSheetName
					}
				}
			});
	}

	void SceneHierarchyPanel::DisplaySpriteRendererComponent(Entity entity)
	{

		DisplayComponent<SpriteRendererComponent>(entity, "Sprite Renderer", [&entity](SpriteRendererComponent& sc)
			{
				if (entity.HasComponent<SpriteComponent>())
				{
					bool& useSprite = sc.UseSprite;
					ImGui::Checkbox("Use Sprite", &useSprite);
				}
			});
	}

	void SceneHierarchyPanel::DisplayCameraComponent(Entity entity)
	{
		using ProjectionType = SceneCamera::ProjectionType;
		DisplayComponent<CameraComponent>(entity, "Camera", [this, entity](auto& cameraComponent)
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

				bool isMainCamera = entity == m_Context.Scene->GetMainCameraEntity();
				if (ImGui::Checkbox("Main Camera", &isMainCamera))
				{
					m_Context.Scene->SetMainCamera(entity);
				}

				switch (currentProjectionType)
				{
				case ProjectionType::Orthographic:
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
				} break;

				case ProjectionType::Perspective:
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
				} break;
				}
			});
	}
}