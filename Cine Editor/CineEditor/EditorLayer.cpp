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
		ScriptEngine::Get().LoadLibary("plugin.dll"); //Temporarily load here.
		m_ActiveScene = CreateRef<Scene>();
		ScriptEngine::Get().InitializeComponents(m_ActiveScene->GetRegistry());


		TextureSpecification iconSpecification;
		iconSpecification.MagFilter = TextureFilter::Linear;
		iconSpecification.MinFilter = TextureFilter::Linear;

		m_IconPlay = Texture2D::Create("Resources/Icons/UI/PlayButton.png", iconSpecification);
		m_IconStop = Texture2D::Create("Resources/Icons/UI/PauseButton.png", iconSpecification);


		FramebufferSpecification spec;
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
		if (m_ViewportHovered)
		{
			m_EditorCamera.OnUpdate(ts);
		}

		Renderer2D::ResetStats();
		m_LastFrametime = ts.Milleseconds();


		m_Framebuffer->Bind();
		{
			switch (m_SceneState)
			{
			case SceneState::Play: m_ActiveScene->OnUpdateRuntime(ts); break;
			case SceneState::Edit: m_ActiveScene->OnUpdateEditor(ts, m_EditorCamera); break;
			}
				
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
		if (m_SceneState == SceneState::Edit)
		{
			Entity selectedEntity = m_SceneHierarchyPanel.GetSelectedEntity();
			if (selectedEntity && m_GizmoOperation > 0)
			{
				auto& hierachy = selectedEntity.GetComponent<HierarchyComponent>();	

				ImGuizmo::SetDrawlist();
				float windowWidth = static_cast<float>(ImGui::GetWindowWidth());
				float windowHeight = static_cast<float>(ImGui::GetWindowHeight());
				ImGuizmo::SetRect(ImGui::GetWindowPos().x, ImGui::GetWindowPos().y, windowWidth, windowHeight);
				;
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
					static_cast<ImGuizmo::OPERATION>(m_GizmoOperation), ImGuizmo::LOCAL, glm::value_ptr(worldTransform),
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

		Ref<Texture2D> icon = m_SceneState == SceneState::Play ? m_IconStop : m_IconPlay;
		ImGui::SameLine((ImGui::GetWindowContentRegionMax().x * 0.5f) - (size * 0.5f));


		if (ImGui::ImageButton("##sceneStateIcon", (ImTextureID)icon->GetRendererID(), { size, size }, {0.0f, 0.0f}, {1.0f, 1.0f}))
		{
			if (m_SceneState == SceneState::Edit)
			{
				OnScenePlay();
			}
			else if (m_SceneState == SceneState::Play)
			{
				OnScenePause();
			}
		}
		ImGui::PopStyleVar(3);
		ImGui::PopStyleColor(3);
		ImGui::End();
	}

	void EditorLayer::OnScenePlay()
	{
		m_SceneState = SceneState::Play;
	}

	void EditorLayer::OnScenePause()
	{
		m_SceneState = SceneState::Edit;
	}

	void EditorLayer::NewScene()
	{
		m_ActiveScene = CreateRef<Scene>();
		ScriptEngine::Get().InitializeComponents(m_ActiveScene->GetRegistry());
		m_SceneHierarchyPanel.SetContext(m_ActiveScene);
		m_ContentBrowserPanel.SetContext(m_ActiveScene);
		m_ActiveScene->OnViewportResize(m_ViewportSize.x, m_ViewportSize.y);
	}
	void EditorLayer::SaveSceneAs()
	{
		std::filesystem::path filepath = FileDialogs::SaveFile("Cine Scene (*.cine)\0*.cine\0");
		if (!filepath.empty())
		{
			SceneSerializer serializer(m_ActiveScene);
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
		if (!path.empty() && path.filename().extension().string() == ".cine")
		{
			std::filesystem::path fullPath = AssetManager::AssetsDirectory / path;
			m_ActiveScene = CreateRef<Scene>();
			ScriptEngine::Get().InitializeComponents(m_ActiveScene->GetRegistry());

			m_SceneHierarchyPanel.SetContext(m_ActiveScene);
			m_ContentBrowserPanel.SetContext(m_ActiveScene);

			SceneSerializer serializer(m_ActiveScene);
			serializer.Deserialize(fullPath);
			m_ActiveScene->OnViewportResize(m_ViewportSize.x, m_ViewportSize.y);
		}
	}



	bool EditorLayer::IsGizmoSnapping() const
	{
		return Internal::Input::IsKeyPressed(Key::LeftControl);
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
		}
		//Gizmos
		if (m_ViewportFocused || m_ViewportHovered)
		{
			switch (e.GetKeyCode())
			{
			case Key::Q:
				m_GizmoOperation = -1;
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