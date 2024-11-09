#include "glash/glash_pch.hpp"
#include "EditorLayer.hpp"

#include "Scene/Components.hpp"
#include "Scene/SceneSerializer.hpp"
#include "Scene/ComponentSerializer.hpp"
#include "glash/Utils/PlatformUtils.hpp"
#include "glash/Core/Timer.hpp"
#include "glash/Math/Math.hpp"

#include <ImGuizmo.h>

namespace Cine
{
	void EditorLayer::OnAttach()
	{
		m_EditorScene = CreateRef<Scene>();
		m_ActiveScene = m_EditorScene;
		ScriptEngine::Get().LoadLibary("plugin.dll");
		ScriptEngine::Get().InitializeComponents(m_ActiveScene->GetRegistry());


		TextureSpecification iconSpecification;
		iconSpecification.MagFilter = TextureFilter::Linear;
		iconSpecification.MinFilter = TextureFilter::Linear;

		m_IconPlay = Texture2D::Create("Resources/Icons/UI/PlayButton.png", iconSpecification);
		m_IconPause = Texture2D::Create("Resources/Icons/UI/PauseButton.png", iconSpecification);
		m_IconStop = Texture2D::Create("Resources/Icons/UI/StopButton.png", iconSpecification);


		FramebufferSpecification spec;
		spec.Attachments = { FramebufferTextureFormat::RGBA8, FramebufferTextureFormat::RED_INTEGER, FramebufferTextureFormat::Depth };
		spec.Width = 1280;
		spec.Height = 720;
		m_Framebuffer = FrameBuffer::Create(spec);

		m_SceneHierarchyPanel.SetContext(m_ActiveScene);
		m_ContentBrowserPanel.SetContext(m_ActiveScene);
	}

	void EditorLayer::OnDetach()
	{

	}

	void EditorLayer::OnUpdate(Timestep ts)
	{
		if (FramebufferSpecification spec = m_Framebuffer->GetSpecification();
			m_ViewportSize.x > 0.0f && m_ViewportSize.y > 0.0f &&
			(spec.Width != m_ViewportSize.x || spec.Height != m_ViewportSize.y))
		{
			m_Framebuffer->Resize((uint32_t)m_ViewportSize.x, (uint32_t)m_ViewportSize.y);
			m_EditorCamera.SetViewportSize(m_ViewportSize.x, m_ViewportSize.y);
			m_ActiveScene->OnViewportResize((uint32_t)m_ViewportSize.x, (uint32_t)m_ViewportSize.y);
		}
		if (m_ViewportHovered && m_SceneState != SceneState::Play)
		{
			m_EditorCamera.OnUpdate(ts);
		}

		Renderer2D::ResetStats();
		m_LastFrametime = ts.Milleseconds();

		m_Framebuffer->Bind();
		RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1.0f });
		RenderCommand::Clear();
		m_Framebuffer->ClearAttachment(1, -1);

		{
			switch (m_SceneState)
			{
			case SceneState::Play: m_ActiveScene->OnUpdateRuntime(ts); break;
			case SceneState::Edit: case SceneState::Pause: m_ActiveScene->OnUpdateEditor(ts, m_EditorCamera); break;
			}
		}

		auto [mx, my] = ImGui::GetMousePos();
		mx -= m_ViewportBounds[0].x;
		my -= m_ViewportBounds[0].y;
		glm::vec2 viewportSize = m_ViewportBounds[1] - m_ViewportBounds[0];

		int mouseX = static_cast<int>(mx);
		int mouseY = static_cast<int>(viewportSize.y - my); //flip Y coordinates

		if (mouseX > 0 && mouseY > 0 && mouseX < (int)viewportSize.x && mouseY < (int)viewportSize.y)
		{
			//Expensive call!
			int pixelData = m_Framebuffer->ReadPixel(1, mouseX, mouseY);
			m_HoveredEntity = pixelData >= 0 ? (Entity(static_cast<entt::entity>(pixelData), m_ActiveScene.get())) : Entity();
		}

		m_Framebuffer->Unbind();
	}

	void EditorLayer::OnEvent(Event& event)
	{
		if (m_ViewportHovered)
		{
			m_EditorCamera.OnEvent(event);

		}

		EventDispatcher dispatcher(event);
		dispatcher.Dispatch<KeyPressedEvent>(CINE_BIND_EVENT_FN(EditorLayer::OnKeyPressed));
		dispatcher.Dispatch<MouseButtonPressedEvent>(CINE_BIND_EVENT_FN(EditorLayer::OnMousePressed));

	}

	void EditorLayer::OnImGuiRender()
	{
		ImGui::DockSpaceOverViewport(0, ImGui::GetMainViewport());

		static int MAX = 10;
		static std::deque <glm::vec3> state_stack;
		static glm::vec3 value(1.0f);
		static glm::vec3 oldValue = value;
		static bool isEdited = false;

		ImGui::Begin("Test");
		{
			bool currentEdited = ImGui::DragFloat3("Value", &value.x);
			bool active = ImGui::IsItemActive();
			isEdited |= currentEdited;

			if (!active && isEdited && value != oldValue)
			{
				state_stack.push_front(oldValue);
				oldValue = value;
				isEdited = false;
				if (state_stack.size() > 10)
				{
					state_stack.pop_back();
				}
			}

			if (ImGui::Button("Undo"))
			{
				if (!state_stack.empty())
				{
					value = state_stack.front();
					state_stack.pop_front();
					oldValue = value;
				}
			}

			for (auto value : state_stack)
			{
				ImGui::Text("%f %f %f", value.x, value.y, value.z);
			}
		}
		ImGui::End();


		static bool show = true;
		ImGui::ShowDemoWindow(&show);

		if (ImGui::BeginMainMenuBar())
		{
			if (ImGui::BeginMenu("File"))
			{
				if (ImGui::MenuItem("New", "Ctrl+N"))
					NewScene();

				if (ImGui::MenuItem("Open...", "Ctrl+O"))
				{
					OpenScene();
				}

				if (ImGui::MenuItem("Save As...", "Ctrl+Shift+S"))
					SaveSceneAs();

				ImGui::EndMenu();
			}
			ImGui::EndMainMenuBar();
		}


		ImGui::Begin("Stats");
		{
			ImGui::Separator();

			std::string hoveredEntityText;
			if (m_HoveredEntity)
			{
				hoveredEntityText = m_HoveredEntity.GetComponent<TagComponent>().Tag + " (" + std::to_string(static_cast<uint32_t>(m_HoveredEntity)) + ")";
			}
			else
			{
				hoveredEntityText = "None";
			}
			ImGui::Text("Hovered Entity: %s", hoveredEntityText.c_str());

			auto stats = Renderer2D::GetStats();
			ImGui::Text("Draw Calls: %llu", stats.DrawCalls);
			ImGui::Text("Quads: %llu", stats.QuadCount);
			ImGui::Text("FrameTime: %.3fms", m_LastFrametime);
			if (ImGui::Button("Close"))
			{
				Application::Get().Close();
			}
		}
		ImGui::End();

		m_SceneHierarchyPanel.OnImGuiRender();
		m_ContentBrowserPanel.OnImGuiRender();

		DrawViewport();

	}

	void EditorLayer::DrawViewport()
	{

		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, { 0.0f, 0.0f });
		ImGui::Begin("Viewport");
		{
			auto viewportMinRegion = ImGui::GetWindowContentRegionMin();
			auto viewportMaxRegion = ImGui::GetWindowContentRegionMax();
			auto viewportOffset = ImGui::GetWindowPos();
			m_ViewportBounds[0] = { viewportMinRegion.x + viewportOffset.x, viewportMinRegion.y + viewportOffset.y };
			m_ViewportBounds[1] = { viewportMaxRegion.x + viewportOffset.x, viewportMaxRegion.y + viewportOffset.y };

			m_ViewportFocused = ImGui::IsWindowFocused();
			m_ViewportHovered = ImGui::IsWindowHovered();
			Application::Get().GetImGuiLayer()->SetBlockEvents(!m_ViewportFocused && !m_ViewportHovered);

			ImVec2 viewportPanelSize = ImGui::GetContentRegionAvail();
			m_ViewportSize = { viewportPanelSize.x, viewportPanelSize.y };

			size_t id = m_Framebuffer->GetColorAttachmentRendererID();
			ImGui::Image(reinterpret_cast<void*>(id), { m_ViewportSize.x, m_ViewportSize.y }, { 0, 1 }, { 1, 0 });

			if (ImGui::BeginDragDropTarget())
			{
				if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("CONTENT_BROWSER_ITEM"))
				{
					const wchar_t* path = (const wchar_t*)payload->Data;
					OpenScene(path);
				}
				ImGui::EndDragDropTarget();
			}
		}

		//Gizmos
		if (m_SceneState == SceneState::Edit || m_SceneState == SceneState::Pause)
		{
			Entity selectedEntity = m_SceneHierarchyPanel.GetSelectedEntity();
			if (selectedEntity && selectedEntity.IsValid() && m_GizmoOperation > 0)
			{
				auto& hierachy = selectedEntity.GetComponent<HierarchyComponent>();

				ImGuizmo::SetDrawlist();
				float windowWidth = static_cast<float>(ImGui::GetWindowWidth());
				float windowHeight = static_cast<float>(ImGui::GetWindowHeight());
				ImGuizmo::SetRect(m_ViewportBounds[0].x, m_ViewportBounds[0].y, m_ViewportBounds[1].x - m_ViewportBounds[0].x, m_ViewportBounds[1].y - m_ViewportBounds[0].y);

				const glm::mat4& cameraProjection = m_EditorCamera.GetProjection();
				const glm::mat4& cameraView = m_EditorCamera.GetViewMatrix();

				auto& tc = selectedEntity.GetComponent<TransformComponent>();
				glm::mat4 worldTransform = tc.GetLocalTransform();
				auto currentEntity = selectedEntity;
				while (currentEntity.HasComponent<HierarchyComponent>())
				{
					auto& hierarchy = currentEntity.GetComponent<HierarchyComponent>();
					if (hierarchy.Parent)
					{
						auto& parentTransform = hierarchy.Parent.GetComponent<TransformComponent>();
						worldTransform = parentTransform.GetLocalTransform() * worldTransform;
						currentEntity = hierarchy.Parent;
					}
					else
					{
						break;
					}
				}

				static bool isOrtho = false;
				ImGuizmo::SetOrthographic(isOrtho);

				bool snap = IsGizmoSnapping();
				float snapValues[3] = { m_SnapValue, m_SnapValue, m_SnapValue };

				ImGuizmo::Manipulate(glm::value_ptr(cameraView), glm::value_ptr(cameraProjection),
					static_cast<ImGuizmo::OPERATION>(m_GizmoOperation), m_GuizmoMode, glm::value_ptr(worldTransform),
					nullptr, snap ? snapValues : nullptr);

				if (ImGuizmo::IsUsing())
				{

					glm::mat4 parentWorldTransform = glm::mat4(1.0f);
					currentEntity = selectedEntity;
					while (currentEntity.HasComponent<HierarchyComponent>())
					{
						auto& hierarchy = currentEntity.GetComponent<HierarchyComponent>();
						if (hierarchy.Parent)
						{
							auto& parentTransform = hierarchy.Parent.GetComponent<TransformComponent>();
							parentWorldTransform = parentTransform.GetLocalTransform() * parentWorldTransform;
							currentEntity = hierarchy.Parent;
						}
						else
						{
							break;
						}
					}
					glm::mat4 localTransform = glm::inverse(parentWorldTransform) * worldTransform;

					glm::vec3 translation, rotation, scale;
					Math::DecomposeTransform(localTransform, translation, rotation, scale);

					tc.Translation = translation;
					tc.Rotation = rotation;
					tc.Scale = scale;

				}
			}
		}
		UI_Toolbar();

		ImGui::End();
		ImGui::PopStyleVar(1);
	}

	void EditorLayer::UI_Toolbar()
	{
		ImGuiWindowFlags toolbarFlags =
			ImGuiWindowFlags_NoDecoration |
			ImGuiWindowFlags_NoScrollbar |
			ImGuiWindowFlags_NoScrollWithMouse;

		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, { 0.0f, 2.0f });
		ImGui::PushStyleVar(ImGuiStyleVar_ItemInnerSpacing, { 0.0f, 0.0f });
		ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, { 0.0f, 0.0f });

		auto& colors = ImGui::GetStyle().Colors;
		ImGui::PushStyleColor(ImGuiCol_Button, { 0.0f, 0.0f, 0.0f, 0.0f });
		auto& buttonHovered = colors[ImGuiCol_ButtonHovered];
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, { buttonHovered.x, buttonHovered.y, buttonHovered.z, 0.5f });
		auto& buttonActive = colors[ImGuiCol_ButtonHovered];
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, { buttonActive.x, buttonActive.y, buttonActive.z, 0.5f });

		ImGuiID viewportDockID = ImGui::GetID("Viewport");
		ImGui::SetNextWindowDockID(viewportDockID, ImGuiCond_FirstUseEver);

		ImGui::Begin("##toolbar", nullptr, toolbarFlags);

		float size = ImGui::GetWindowHeight() - 4.0f;
		Ref<Texture2D> playPauseIcon = m_SceneState == SceneState::Play ? m_IconPause : m_IconPlay;

		float totalWidth = size;
		bool hasStopButton = (m_SceneState == SceneState::Play || m_SceneState == SceneState::Pause);
		if (hasStopButton)
		{
			totalWidth += size + ImGui::GetStyle().ItemSpacing.x;
		}

		ImGui::SameLine((ImGui::GetWindowContentRegionMax().x * 0.5f) - (totalWidth * 0.5f));

		if (ImGui::ImageButton("##sceneStateIcon", (ImTextureID)(uint64_t)playPauseIcon->GetRendererID(), { size, size }, { 0.0f, 0.0f }, { 1.0f, 1.0f }))
		{
			if (m_SceneState == SceneState::Edit || m_SceneState == SceneState::Pause)
			{
				OnScenePlay();
			}
			else if (m_SceneState == SceneState::Play)
			{
				OnScenePause();
			}
		}

		if (hasStopButton)
		{
			ImGui::SameLine();
			if (ImGui::ImageButton("##stopIcon", (ImTextureID)(uint64_t)m_IconStop->GetRendererID(), { size, size }, { 0.0f, 0.0f }, { 1.0f, 1.0f }))
			{
				OnSceneStop();
			}
		}

		ImGui::PopStyleVar(3);
		ImGui::PopStyleColor(3);
		ImGui::End();
	}



	void EditorLayer::OnScenePlay()
	{
		if (m_SceneState == SceneState::Play)
		{
			return;
		}

		if (m_SceneState == SceneState::Edit)
		{
			std::string tempSceneName = m_EditorScene->GetName() + ".runtime";
			SceneSerializer serializer(m_EditorScene);
			serializer.Serialize(tempSceneName);
			m_RuntimeScene = CreateRef<Scene>();
			ScriptEngine::Get().SetActiveRegistry(m_RuntimeScene->GetRegistry());
			SceneSerializer deserializer(m_RuntimeScene);
			deserializer.Deserialize(tempSceneName);
		}

		m_ActiveScene = m_RuntimeScene;
		ScriptEngine::Get().SetActiveRegistry(m_ActiveScene->GetRegistry());
		m_ActiveScene->OnRuntimeStart();
		m_ActiveScene->OnViewportResize(m_ViewportSize.x, m_ViewportSize.y);

		m_SceneHierarchyPanel.SetContext(m_ActiveScene);
		m_ContentBrowserPanel.SetContext(m_ActiveScene);

		m_SceneState = SceneState::Play;
	}

	void EditorLayer::OnScenePause()
	{
		m_ActiveScene->OnRuntimePause();
		m_SceneState = SceneState::Pause;
	}

	void EditorLayer::OnSceneStop()
	{
		m_ActiveScene->OnRuntimeStop();

		m_SceneHierarchyPanel.SetContext(m_EditorScene);
		m_ContentBrowserPanel.SetContext(m_EditorScene);

		m_ActiveScene.reset();
		m_RuntimeScene.reset();

		ScriptEngine::Get().SetActiveRegistry(m_EditorScene->GetRegistry());


		m_ActiveScene = m_EditorScene;

		m_SceneState = SceneState::Edit;
	}

	void EditorLayer::NewScene()
	{
		if (m_SceneState == SceneState::Edit)
		{
			m_EditorScene = CreateRef<Scene>();
			ScriptEngine::Get().InitializeComponents(m_EditorScene->GetRegistry());
			m_SceneHierarchyPanel.SetContext(m_EditorScene);
			m_ContentBrowserPanel.SetContext(m_EditorScene);
			m_EditorScene->OnViewportResize(m_ViewportSize.x, m_ViewportSize.y);

			m_ActiveScene = m_EditorScene;
		}

	}
	void EditorLayer::SaveSceneAs()
	{
		std::filesystem::path filepath = FileDialogs::SaveFile("Cine Scene (*.cine)\0*.cine\0");
		if (!filepath.empty())
		{
			SceneSerializer serializer(m_EditorScene);
			serializer.Serialize(filepath);
		}
	}
	void EditorLayer::OpenScene()
	{
		std::filesystem::path path = FileDialogs::OpenFile("Cine Scene (*.cine)\0*.cine\0");
		OpenScene(path);
	}

	void EditorLayer::OpenScene(const std::filesystem::path& path)
	{
		if (m_SceneState != SceneState::Edit)
		{
			return;
		}

		if (!path.empty() && path.filename().extension().string() == ".cine")
		{
			m_EditorScene = CreateRef<Scene>();
			m_ActiveScene = m_EditorScene; //don't move this piece of shit!
			m_SceneHierarchyPanel.SetContext(m_EditorScene);
			m_ContentBrowserPanel.SetContext(m_EditorScene);

			ScriptEngine::Get().InitializeComponents(m_EditorScene->GetRegistry());

			std::filesystem::path fullPath = AssetManager::AssetsDirectory / path;

			SceneSerializer serializer(m_EditorScene);
			serializer.Deserialize(fullPath);

			m_EditorScene->OnViewportResize(m_ViewportSize.x, m_ViewportSize.y);

			//OnSceneStop();
		}
	}



	bool EditorLayer::IsGizmoSnapping() const
	{
		return Internal::Input::IsKeyPressed(Key::LeftControl);
	}

	bool EditorLayer::OnMousePressed(MouseButtonPressedEvent& e)
	{
		if (m_IsRuntime) return false;

		//Check for gizmo operation
		if (m_GizmoOperation > -1)
			return false;

		if (e.GetMouseButton() == Mouse::ButtonLeft && m_HoveredEntity)
		{
			if (m_SceneHierarchyPanel.GetSelectedEntity() == m_HoveredEntity)
			{
				m_SceneHierarchyPanel.SetPropertiesEntity(m_HoveredEntity);
			}
			else
			{
				m_SceneHierarchyPanel.SetSelectedEntity(m_HoveredEntity);
			}
		}

		return false;
	}

	bool EditorLayer::OnKeyPressed(KeyPressedEvent& e)
	{
		if (m_IsRuntime) return false;

		if (e.GetRepeatCount() > 0)
		{
			return false;
		}
		bool control = Internal::Input::IsKeyPressed(Key::LeftControl) || Internal::Input::IsKeyPressed(Key::RightControl);
		bool shift = Internal::Input::IsKeyPressed(Key::LeftShift) || Internal::Input::IsKeyPressed(Key::RightShift);

		//Shortcuts
		switch (e.GetKeyCode())
		{
		case Key::N:
		{
			if (control)
				NewScene();
		} break;
		case Key::O:
		{
			if (control)
			{
				OpenScene();
			}
		} break;
		case Key::S:
		{
			if (control && shift)
				SaveSceneAs();
		} break;
		case Key::KP0:
		{
			m_EditorCamera.Reset();
		} break;

		}

		//Gizmos
		if (m_ViewportFocused || m_ViewportHovered)
		{
			switch (e.GetKeyCode())
			{
			case Key::G:
				m_GuizmoMode = m_GuizmoMode == ImGuizmo::MODE::LOCAL ? ImGuizmo::MODE::WORLD : ImGuizmo::MODE::LOCAL;
				break;
			case Key::Q:
				m_GizmoOperation = -1; //For future selection mode
				break;
			case Key::W:
				m_GizmoOperation = ImGuizmo::OPERATION::TRANSLATE;
				m_SnapValue = m_SnapTranslation;
				break;
			case Key::E:
				m_GizmoOperation = ImGuizmo::OPERATION::ROTATE;
				m_SnapValue = m_SnapRotation;
				break;
			case Key::R:
				m_GizmoOperation = ImGuizmo::OPERATION::SCALE;
				m_SnapValue = m_SnapScale;
				break;
			case Key::X:
			case Key::Y:
			case Key::Z:
			{
				if (m_GizmoOperation < 0) break;
				ImGuizmo::OPERATION baseOperation = ImGuizmo::OPERATION::TRANSLATE | ImGuizmo::OPERATION::ROTATE | ImGuizmo::OPERATION::SCALE;
				ImGuizmo::OPERATION currentOperation = static_cast<ImGuizmo::OPERATION>(m_GizmoOperation);

				if ((currentOperation & baseOperation) != 0)
				{
					if (currentOperation & ImGuizmo::OPERATION::TRANSLATE)
					{
						m_GizmoOperation = (Key::X == e.GetKeyCode()) ? static_cast<int>(ImGuizmo::OPERATION::TRANSLATE_X) :
							(Key::Y == e.GetKeyCode()) ? static_cast<int>(ImGuizmo::OPERATION::TRANSLATE_Y) :
							static_cast<int>(ImGuizmo::OPERATION::TRANSLATE_Z);
					}
					else if (currentOperation & ImGuizmo::OPERATION::ROTATE)
					{
						m_GizmoOperation = (Key::X == e.GetKeyCode()) ? static_cast<int>(ImGuizmo::OPERATION::ROTATE_X) :
							(Key::Y == e.GetKeyCode()) ? static_cast<int>(ImGuizmo::OPERATION::ROTATE_Y) :
							static_cast<int>(ImGuizmo::OPERATION::ROTATE_Z);
					}
					else if (currentOperation & ImGuizmo::OPERATION::SCALE)
					{
						m_GizmoOperation = (Key::X == e.GetKeyCode()) ? static_cast<int>(ImGuizmo::OPERATION::SCALE_X) :
							(Key::Y == e.GetKeyCode()) ? static_cast<int>(ImGuizmo::OPERATION::SCALE_Y) :
							static_cast<int>(ImGuizmo::OPERATION::SCALE_Z);
					}
				}
				else
				{
					m_GizmoOperation = (Key::X == e.GetKeyCode()) ? static_cast<int>(ImGuizmo::OPERATION::TRANSLATE_X) :
						(Key::Y == e.GetKeyCode()) ? static_cast<int>(ImGuizmo::OPERATION::TRANSLATE_Y) :
						(Key::Z == e.GetKeyCode()) ? static_cast<int>(ImGuizmo::OPERATION::TRANSLATE_Z) : m_GizmoOperation;
				}
			}
			break;

			default:
				break;
			}
		}
		return false;
	}

}