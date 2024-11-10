#include "glash/glash_pch.hpp"
#include "SceneHierarchyPanel.hpp"
#include "glash/Cine.hpp"

#include "glash/Scene/Components.hpp"
#include "glash/Scene/SceneSerializer.hpp"
#include "glash/Scene/AssetManager.hpp"

#include "../Utils/Shell.hpp"

#include "Dialog.hpp"

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

		ImGuiTreeNodeFlags treeNodeFlags =
			ImGuiTreeNodeFlags_AllowItemOverlap
			| ImGuiTreeNodeFlags_Framed
			| ImGuiTreeNodeFlags_FramePadding
			| ImGuiTreeNodeFlags_SpanAvailWidth;
		
		if constexpr (std::is_same<T, TransformComponent>::value || std::is_same<T, TagComponent>::value)
		{
			treeNodeFlags |= ImGuiTreeNodeFlags_DefaultOpen;
		}

		ImVec2 contentRegionAvailable = ImGui::GetContentRegionAvail();

		ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, { 4.0f, 4.0f });
		float lineHeigh = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.0f;
		ImGui::Separator();

		bool open = ImGui::TreeNodeEx((void*)typeid(T).hash_code(), treeNodeFlags, name);
		ImGui::PopStyleVar();

		bool removeComponent = false;
		if constexpr (!IsNonRemovableComponent<T>::value)
		{
			static const std::string uniqueButtonName = "+##" + Utils::GetClassTypename<T>();
			static const std::string uniquePopupID = "ComponentSettings##" + Utils::GetClassTypename<T>();
			ImGui::SameLine(contentRegionAvailable.x - lineHeigh * 0.5f);
			if (ImGui::Button(uniqueButtonName.c_str(), {lineHeigh, lineHeigh}))
			{
				ImGui::OpenPopup(uniquePopupID.c_str());
			}
			if (ImGui::BeginPopup(uniquePopupID.c_str()))
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

	static bool DisplayVec3Control(const std::string& label, glm::vec3& values, float resetValue = 0.0f, float columnWidth = 100.0f)
	{
		bool updated = false;

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
			{
				value = resetValue;
				updated = true;
			}

			ImGui::PopFont();
			ImGui::PopStyleColor(3);
			ImGui::SameLine();
			if (ImGui::DragFloat(("##" + std::string(label)).c_str(), &value, 0.1f, 0.0f, 0.0f, "%.2f"))
				updated = true;
			ImGui::PopItemWidth();
			ImGui::SameLine();
			};

		drawControl("X", values.x, ImVec4{ 0.8f, 0.1f, 0.15f, 1.0f });
		drawControl("Y", values.y, ImVec4{ 0.2f, 0.7f, 0.3f, 1.0f });
		drawControl("Z", values.z, ImVec4{ 0.1f, 0.25f, 0.8f, 1.0f });

		ImGui::PopStyleVar();

		ImGui::Columns(1);
		ImGui::PopID();

		return updated;
	}


	SceneHierarchyPanel::SceneHierarchyPanel(const Ref<Scene>& scene)
	{
		SetContext(scene);
	}

	void SceneHierarchyPanel::SetContext(const Ref<Scene>& scene)
	{
		m_Context.Scene = scene;
		auto selection = m_Context.Selection;
		auto properties = m_Context.Properties;
		
		m_Context.Selection = {};
		m_Context.Properties = {};

		if (selection && selection.IsValid())
		{
			UUID selectedEntityID = selection.GetID();
			Entity selectedEntity = m_Context.Scene->GetEntityById(selectedEntityID);
			if (selectedEntity)
			{
				m_Context.Selection = selectedEntity;
			}
		}

		if (properties && properties.IsValid())
		{
			UUID propertiesEntityID = properties.GetID();
			Entity propertiesEntity = m_Context.Scene->GetEntityById(propertiesEntityID);
			if (propertiesEntity)
			{
				m_Context.Properties = propertiesEntity;
			}
		}
	}

	void SceneHierarchyPanel::SetSelectedEntity(Entity entity)
	{
		if (entity.IsValid())
		{
			m_Context.Selection = entity;
		}
	}

	void SceneHierarchyPanel::SetPropertiesEntity(Entity entity)
	{
		if (entity.IsValid())
		{
			m_Context.Properties = entity;
		}
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

				if (!entity.IsValid())
				{
					if (m_Context.Selection == entity)
					{
						m_Context.Selection = {};
					}
					if (m_Context.Properties == entity)
					{
						m_Context.Properties = {};
					}
					continue;
				}

				if (entity.HasComponent<HierarchyComponent>() && !entity.GetComponent<HierarchyComponent>().Parent)
				{
					DisplayEntityNode(entity);
				}
			}

			//Dummy for drag and drop into empty space
			ImVec2 availableSpace = ImGui::GetContentRegionAvail();
			float minHeight = 40.0f;
			ImGui::Dummy(ImVec2(availableSpace.x, std::max(minHeight, availableSpace.y)));

			if (ImGui::BeginDragDropTarget())
			{
				if (ImGui::AcceptDragDropPayload("ENTITY"))
				{
					Entity& draggedEntity = *reinterpret_cast<Entity*>(ImGui::GetDragDropPayload()->Data);

					if (draggedEntity)
					{
						draggedEntity.RemoveParent();
					}
				}
				ImGui::EndDragDropTarget();
			}
		}

		if (ImGui::IsMouseDoubleClicked(0) && ImGui::IsWindowHovered() && !ImGui::IsAnyItemHovered())
		{
			m_Context.Selection = {};
			m_Context.Properties = {};
		}

		if (ImGui::BeginPopupContextWindow(0, ImGuiPopupFlags_MouseButtonDefault_ | ImGuiPopupFlags_NoOpenOverItems))
		{
			if (ImGui::MenuItem("Create Entity"))
			{
				Entity newEntity = m_Context.Scene->CreateEntity("New Entity");
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
				AddComponentItem<NativeScriptComponent>(m_Context.Properties, "Native Script");
				AddComponentItem<RigidBody2DComponent>(m_Context.Properties, "Rigid Body 2D");
				AddComponentItem<BoxCollider2DComponent>(m_Context.Properties, "Box Collider 2D");

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
		auto& tag = entity.GetComponent<TagComponent>().Tag;
		auto& hierarchyComponent = entity.GetComponent<HierarchyComponent>();

		ImGuiTreeNodeFlags flags = ((m_Context.Selection == entity) ? ImGuiTreeNodeFlags_OpenOnArrow : 0);
		flags |= ImGuiTreeNodeFlags_SpanAvailWidth;
		if (m_Context.Selection == entity)
		{
			flags |= ImGuiTreeNodeFlags_Selected;
		}

		if (hierarchyComponent.Children.empty()) {
			flags |= ImGuiTreeNodeFlags_Leaf;
		}

		static std::unordered_map<uint32_t, bool> expandedNodes;
		bool& isExpanded = expandedNodes[(uint32_t)entity];

		ImGui::SetNextItemOpen(isExpanded);

		bool expanded = ImGui::TreeNodeEx((const void*)(uint64_t)(uint32_t)entity, flags, tag.c_str());

		if (ImGui::BeginDragDropSource())
		{
			ImGui::SetDragDropPayload("ENTITY", &entity, sizeof(Entity));
			ImGui::Text(tag.c_str());
			ImGui::EndDragDropSource();
		}

		if (ImGui::BeginDragDropTarget())
		{
			if (ImGui::AcceptDragDropPayload("ENTITY"))
			{
				Entity* draggedEntity = reinterpret_cast<Entity*>(ImGui::GetDragDropPayload()->Data);
				if (draggedEntity && *draggedEntity != entity)
				{
					if (draggedEntity->AddParent(entity))
					{
						isExpanded = true;
					}
				}
			}
			ImGui::EndDragDropTarget();
		}

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
			ImVec4 cloneButtonColor = { 0.25f, 1.0f, 0.35f, 1.0f };
			ImGui::PushStyleColor(ImGuiCol_Text, cloneButtonColor);
			if (ImGui::MenuItem("Clone"))
			{
				Entity clone = entity.Clone();
				clone.GetComponent<TagComponent>().Tag = entity.GetComponent<TagComponent>().Tag + " (Clone)";
				if (entity == m_Context.Selection)
				{
					m_Context.Selection = clone;
				}

				if (entity == m_Context.Properties)
				{
					m_Context.Properties = clone;
				}
			}
			ImGui::PopStyleColor();
			ImVec4 deleteButtonColor = { 1.0f, 0.15f, 0.15f, 1.0f };
			ImGui::PushStyleColor(ImGuiCol_Text, deleteButtonColor);
			if (ImGui::MenuItem("Delete"))
			{
				entityDeleted = true;
			}
			ImGui::PopStyleColor();
			ImGui::EndPopup();
		}

		if (expanded)
		{
			isExpanded = true;
			for (auto child : hierarchyComponent.Children)
			{
				DisplayEntityNode(child);
			}
			ImGui::TreePop();
		}
		else if (!expanded && isExpanded)
		{
			isExpanded = false;
		}

		if (entityDeleted)
		{
			m_Context.Scene->DestroyEntity(entity);
			if (m_Context.Selection == entity)
			{
				m_Context.Selection = {};
			}
			if (m_Context.Properties == entity)
			{
				m_Context.Properties = {};
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

		if (entity.HasComponent<RigidBody2DComponent>())
		{
			DisplayRigidBody2DComponent(entity);
		}

		if (entity.HasComponent<BoxCollider2DComponent>())
		{
			DisplayBoxCollider2DComponent(entity);
		}

		if (entity.HasComponent<NativeScriptComponent>())
		{
			DisplayNativeScriptComponent(entity);
		}
	}

	void SceneHierarchyPanel::DisplayTransformComponent(Entity entity)
	{
		bool updated = false;
		ImGui::IsItemDeactivated();
		DisplayComponent<TransformComponent>(entity, "Transform", [&](TransformComponent& tc)
			{
				auto& translation = tc.Translation;
				updated |= DisplayVec3Control("Position", translation);

				ImGui::Spacing();

				auto rotation = glm::degrees(tc.Rotation);
				updated |= DisplayVec3Control("Rotation", rotation);
				tc.Rotation = glm::radians(rotation);

				ImGui::Spacing();

				auto& scale = tc.Scale;
				updated |= DisplayVec3Control("Scale", scale, 1.0f);


				if(updated)
					m_Context.Scene->GetPhysics2DSystem().UpdateRigidBodyParameters(entity);
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
					int32_t maxIndex = static_cast<int32_t>(entity.GetComponent<SpriteSheetComponent>().Frames.size()) - 1;
					if (maxIndex >= 0)
					{
						ImGui::SliderInt("Sprite Sheet Index", &sprite.SpriteIndex, 0, maxIndex);
					}
					else
					{
						ImGui::BeginDisabled();
						int32_t dummy = 0;
						ImGui::SliderInt("Sprite Sheet Index", &dummy, 0, 0);
						ImGui::EndDisabled();
					}
				}
			});

	}


	void SceneHierarchyPanel::DisplaySpriteSheetComponent(Entity entity)
	{
		DisplayComponent<SpriteSheetComponent>(entity, "Sprite Sheet", [&entity](SpriteSheetComponent& spriteSheet)
			{
				static bool displayFrames = false;
				ImGui::Checkbox("Display frames", &displayFrames);
				if (spriteSheet.Texture && displayFrames)
				{
					constexpr float step = 32.0f;
					float texWidth = static_cast<float>(spriteSheet.Texture->GetWidth());
					float texHeight = static_cast<float>(spriteSheet.Texture->GetHeight());

					for (size_t i = 0; i < spriteSheet.Frames.size(); ++i) {
						auto& frame = spriteSheet.Frames[i];
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
							spriteSheet.Frames.erase(spriteSheet.Frames.begin() + i);
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
						spriteSheet.Frames.push_back(newFrame);
					}
				}
				if (spriteSheet.Texture && ImGui::Button("Save SpriteSheet"))
				{
					ImGui::OpenPopup("Save sprite sheet meta");
				}
				ShowConfirmationPopup(
					"Save sprite sheet meta",
					"Are you sure you want to save the sprite sheet meta? It will overwrite existing data",
					[&spriteSheet]() { AssetManager::SaveSpriteSheetMeta(spriteSheet); }
				);
				if (!spriteSheet.Texture)
				{
					ImGui::BeginDisabled();
					ImGui::Button("Save SpriteSheet");
					ImGui::EndDisabled();
				}
				ImGui::SameLine();
				if (ImGui::Button("Load SpriteSheet"))
				{
					std::filesystem::path filepath = FileDialogs::OpenFile("Sprite Sheet (*.png)\0*.png\0");
					if (!filepath.empty())
					{
						spriteSheet = AssetManager::LoadSpriteSheet("dummy", filepath); //TODO: SpriteSheetName
					}
				}


			});
	}

	void SceneHierarchyPanel::DisplaySpriteRendererComponent(Entity entity)
	{

		DisplayComponent<SpriteRendererComponent>(entity, "Sprite Renderer", [&entity](SpriteRendererComponent& sc)
			{
				if (entity.HasComponents<SpriteComponent, SpriteSheetComponent>() && entity.GetComponent<SpriteSheetComponent>().Texture)
				{
					bool& useSprite = sc.UseSprite;
					ImGui::Checkbox("Use Sprite", &useSprite);
				}
				else
				{
					ImGui::BeginDisabled();
					bool dummy = false;
					ImGui::Checkbox("Use Sprite", &dummy);
					ImGui::EndDisabled();
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
	void SceneHierarchyPanel::DisplayBoxCollider2DComponent(Entity entity)
	{
		DisplayComponent<BoxCollider2DComponent>(entity, "Box Collider 2D", [this, entity](BoxCollider2DComponent& collider)
			{
				bool valueChanged = false;
				valueChanged |= ImGui::Checkbox("Is Trigger", &collider.IsTrigger);
				valueChanged |= ImGui::DragFloat2("Offset", glm::value_ptr(collider.Offset), 0.01f, 0.0f, 0.0f, "%.2f");
				valueChanged |= ImGui::DragFloat2("Size", glm::value_ptr(collider.Size), 0.01f, 0.0f, 0.0f, "%.2f");

				valueChanged |= ImGui::DragFloat("Density", &collider.Density, 0.01, 0.0f, 0.0f, "%.2f");
				valueChanged |= ImGui::DragFloat("Friction", &collider.Friction, 0.01, 0.0f, 1.0f, "%.2f");
				valueChanged |= ImGui::DragFloat("Restitution", &collider.Restitution, 0.01f, 0.0f, 1.0f, "%.2f");
				valueChanged |= ImGui::DragFloat("Restituion Threasold", &collider.RestitutionThreshold, 0.01f, 0.0f, 1.0f, "%.2f");

				if (valueChanged && m_Context.Scene->IsRuntime())
				{
					m_Context.Scene->GetPhysics2DSystem().UpdateRigidBodyParameters(entity);
				}
			});
	}

	void SceneHierarchyPanel::DisplayRigidBody2DComponent(Entity entity)
	{
		DisplayComponent<RigidBody2DComponent>(entity, "Rigid Body 2D", [this, entity](RigidBody2DComponent& rb)
			{
				bool valueChanged = false;
				std::array<const char*, 3> typeStrigns = { "Static", "Dynamic", "Kinematic"};
				const char* currentType = typeStrigns[static_cast<int>(rb.Type)];

				if (ImGui::BeginCombo("Body Type", currentType))
				{
					for (int i = 0; i < typeStrigns.size(); ++i)
					{
						bool isSelected = currentType == typeStrigns[i];
						if (ImGui::Selectable(typeStrigns[i], &isSelected))
						{
							valueChanged |= true;

							currentType = typeStrigns[i];
							rb.Type = static_cast<RigidBody2DComponent::BodyType>(i);
						}
						if (isSelected)
						{
							ImGui::SetItemDefaultFocus();
						}
					}
					ImGui::EndCombo();
				}

				if (ImGui::Checkbox("Fixed Rotation", &rb.FixedRotation))
				{
					valueChanged |= true;
				}

				if (valueChanged && m_Context.Scene->IsRuntime())
				{
					m_Context.Scene->GetPhysics2DSystem().UpdateRigidBodyParameters(entity);
				}
			});
	}
	void SceneHierarchyPanel::DisplayNativeScriptComponent(Entity entity)
	{
		DisplayComponent<NativeScriptComponent>(entity, "Native Scripts", [this, entity](NativeScriptComponent& nsc)
			{
				if (!nsc.Scripts.size() == 0)
				{
					for (auto& script : nsc.Scripts)
					{
						if (!script.Instance)
						{
							continue;
						}

						bool& enabled = script.Instance->Enabled;

						std::string buttonIcon = "X##" + script.Name;
						if (ImGui::Button(buttonIcon.c_str()))
						{
							m_Context.Scene->RemoveComponentByName(entity, script.Name);
							return;
						}
						ImGui::Text("%s", script.Name.c_str());

						ImGui::SameLine();

						float maxContentWidth = ImGui::GetContentRegionMax().x;

						float checkboxWidth = ImGui::CalcTextSize("Enabled").x + ImGui::GetStyle().FramePadding.x * 10;

						ImGui::SetCursorPosX(maxContentWidth - checkboxWidth);

						std::string EnabledField = "Enabled##" + script.Name;
						ImGui::Checkbox(EnabledField.c_str(), &enabled);

						YAML::Node serialized = m_Context.Scene->SerializeComponentByName(entity, script.Name);
						YAML::Node node = serialized[script.Name];

						if (!node)
						{
							continue;
						}

						for (auto key : node)
						{
							const std::string name = key.first.as<std::string>();
							YAML::Node fieldNode = key.second;

							std::string fieldName = name + "##" + script.Name;
							if (fieldNode.IsScalar())
							{
								std::string stringValue = fieldNode.as<std::string>();
								std::string value_copy = stringValue;

								int intValue = 0;
								float floatValue = 0.0f;
								bool boolValue = false;

								bool isInt = false;
								bool isFloat = false;
								bool isBool = false;

								if (Math::IsInteger(stringValue)) {
									intValue = std::stoi(stringValue);
									isInt = true;
								}
								else if (Math::IsFloat(stringValue)) {
									floatValue = std::stof(stringValue);
									isFloat = true;
								}
								else if (Math::IsBool(stringValue))
								{
									boolValue = stringValue == "True" || stringValue == "true" ? true : false;
									isBool = true;
								}

								bool valueChanged = false;
								if (isInt)
								{
									if (ImGui::DragInt(fieldName.c_str(), &intValue, 1.0f, std::numeric_limits<int>::lowest(), std::numeric_limits<int>::max()))
									{
										stringValue = std::to_string(intValue);
										valueChanged = true;
									}
								}
								else if (isFloat)
								{
									if (ImGui::DragFloat(fieldName.c_str(), &floatValue, 0.1f, std::numeric_limits<float>::lowest(), std::numeric_limits<float>::lowest(), "%.3f"))
									{
										stringValue = std::to_string(floatValue);
										valueChanged = true;
									}
								}
								else if (isBool)
								{
									if (ImGui::Checkbox(fieldName.c_str(), &boolValue))
									{
										stringValue = boolValue ? "True" : "False";
										valueChanged = true;
									}
								}
								else
								{
									if (ImGui::InputText(fieldName.c_str(), const_cast<char*>(stringValue.c_str()), stringValue.length()))
									{
										valueChanged = true;
									}
								}

								if (valueChanged)
								{
									try
									{
										fieldNode = stringValue.c_str();
										serialized[script.Name] = node;
										m_Context.Scene->DeserializeComponentByName(entity, script.Name, serialized);
									}
									catch (const std::exception& e)
									{
										// Revert the value if deserialization fails
										fieldNode = value_copy.c_str();
										serialized[script.Name] = node;
										m_Context.Scene->DeserializeComponentByName(entity, script.Name, serialized);
									}
									script.Instance->Object = entity; //Currently Instnace is recreated losing the Object reference.
								}
							}
						}
						ImGui::Separator();

					}
				}


				auto& componetsData = m_Context.Scene->GetComponentsData();
				auto& scripts = nsc.Scripts;

				static char searchBuffer[128] = "";

				if (ImGui::Button("Add Script"))
				{
					ImGui::OpenPopup("AddScriptPopup");
				}

				static bool showCreationResult = false;
				static bool creationSuccess = false;
				static std::string creationMessage;
				static float notificationTimer = 0.0f;
				const float notificationDuration = 5.0f;

				if (ImGui::BeginPopup("AddScriptPopup"))
				{
					ImGui::InputText("Search", searchBuffer, IM_ARRAYSIZE(searchBuffer));

					ImGui::SameLine();
					ImGui::Dummy(ImVec2(10.0f, 0.0f));
					ImGui::SameLine();

					if (ImGui::Button("Create"))
					{
						std::string scriptName = searchBuffer;
						scriptName += ".hpp";
						std::filesystem::path scriptFilePath = AssetManager::AssetsDirectory / "Scripts" / scriptName;

						if (std::filesystem::exists(scriptFilePath))
						{
							creationSuccess = false;
							creationMessage = "Script already exists.";
						}
						else
						{
							Shell::CreateNewScript(searchBuffer);

							if (std::filesystem::exists(scriptFilePath))
							{
								creationSuccess = true;
							}

							if (creationSuccess)
							{
								creationMessage = "Script created successfully!";
								ZeroMemory(searchBuffer, sizeof(searchBuffer));
							}
							else
							{
								creationMessage = "Failed to create the script.";
							}
						}

						showCreationResult = true;
						notificationTimer = notificationDuration;
						ImGui::CloseCurrentPopup();
					}

					for (auto&& [name, isScript] : componetsData)
					{
						if (!isScript)
						{
							continue;
						}

						auto it = std::find_if(scripts.begin(), scripts.end(), [&name](auto& script)
							{
								return script.Name == name;
							});
						bool hasScript = it != scripts.end();

						if (hasScript)
							ImGui::BeginDisabled();

						std::string nameCopy(name);
						std::string searchValue = searchBuffer;
						
						std::transform(nameCopy.begin(), nameCopy.end(), nameCopy.begin(), ::tolower);
						std::transform(searchValue.begin(), searchValue.end(), searchValue.begin(), ::tolower);

						if (nameCopy.find(searchValue) != std::string::npos)
						{
							ImVec2 buttonSize = ImVec2(200.0f, 0.0f);

							if (ImGui::Button(name.c_str(), buttonSize))
							{
								m_Context.Scene->AddComponentByName(entity, name);
								ImGui::CloseCurrentPopup();
							}
						}

						if (hasScript)
							ImGui::EndDisabled();
					}

					ImGui::EndPopup();
				}

				if (showCreationResult)
				{
					ShowNotification(creationMessage, notificationTimer, notificationDuration);
					if (notificationTimer <= 0.0f)
					{
						showCreationResult = false;
					}
				}
			});

	}
}