#include "glash/glash_pch.hpp"
#include "EditorLayer.hpp"

#include "Scene/Components.hpp"
#include "Scene/SceneSerializer.hpp"
#include "glash/Utils/PlatformUtils.hpp"
#include "glash/Core/Timer.hpp"

#include "glash/Math/Math.hpp"

#include <ImGuizmo.h>

struct Health {
	int value;
};

struct Position {
	float x, y;
};

struct Name {
	std::string value;
};


static Cine::Scene* s_Scene = nullptr;

namespace Cine
{
	class ColorScript : public NativeScript
	{
	public:
		ColorScript()
		{
			++counter;
			CINE_LOG_INFO("ColorScript Counter: {}", counter);
		}
		void OnCreate() override
		{
			counter = 0;
			auto& component = GetComponent<SpriteRendererComponent>();
			m_SpriteRendererComponent = &component;
			m_Timer.Start();
		}

		void OnUpdate(Timestep ts) override
		{
			m_Timer.OnUpdate(ts);

			float time = m_Timer.GetElapsed();
			m_SpriteRendererComponent->Color.r = 0.5f * sin(time) + 0.5f;
			m_SpriteRendererComponent->Color.g = 0.5f * sin(time + 2.0f) + 0.5f;
			m_SpriteRendererComponent->Color.b = 0.5f * sin(time + 4.0f) + 0.5f;
		}
	public:
		std::string String = "String from Color Script";

	private:
		SpriteRendererComponent* m_SpriteRendererComponent;
		Timer m_Timer;
		static int counter;
	};

	int ColorScript::counter = 0;

	class ControllerScript : public NativeScript
	{
	public:
		void OnCreate() override
		{

		}

		void OnDestroy() override
		{

		}

		void OnUpdate(Timestep ts) override
		{

			auto& transform = GetComponent<TransformComponent>().Translation;

			float speed = 5.0f;

			if (Input::IsKeyPressed(Key::A))
			{
				transform.x -= speed * ts;
			}
			if (Input::IsKeyPressed(Key::D))
			{
				transform.x += speed * ts;
			}
			if (Input::IsKeyPressed(Key::W))
			{
				transform.y += speed * ts;
			}
			if (Input::IsKeyPressed(Key::S))
			{
				transform.y -= speed * ts;
			}

		}
	};

	void EditorLayer::OnAttach()
	{
		FramebufferSpecification spec;
		spec.Width = 1280;
		spec.Height = 720;
		m_Framebuffer = FrameBuffer::Create(spec);


		m_ActiveScene = CreateRef<Scene>();
		s_Scene = m_ActiveScene.get();

		m_SceneHierarchyPanel.SetContext(m_ActiveScene);

		m_EditorCamera = EditorCamera(45.0f, 16.0f / 9.0f, 0.01f, 1000.0f);

		auto entity = m_ActiveScene->CreateEntity("Sprite Test");

		m_TextureLibrary.LoadTexture2D("Thing", "Assets/Textures/SpriteSheet.png");

		auto& sr = entity.AddComponent<SpriteRendererComponent>();
		auto& sheet = entity.AddComponent<SpriteSheetComponent>();

		s_Scene->RegisterComponent<ControllerScript>();
		s_Scene->RegisterComponent<ColorScript>();

		entity.AddComponentByName("ColorScript");
		entity.AddComponentByName("ControllerScript");


		sheet.Texture = m_TextureLibrary.GetTexture2D("Thing");
		Sprite::Frame frame = { 0, 129, 128, 128 * 2 };
		auto& tr = entity.GetComponent<TransformComponent>();
		tr.Scale.y = frame.height / frame.width;
		sheet.Frames.push_back(frame);
		sr.SpriteSheetIndex = 0;
		sr.UseSprite = true;

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
		m_LastFrameTime = ts.Milleseconds();


		m_Framebuffer->Bind();
		{
			m_ActiveScene->OnUpdateEditor(ts, m_EditorCamera);
			//m_ActiveScene->OnUpdateRuntime(ts);
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
					OpenScene();

				if (ImGui::MenuItem("Save As...", "Ctrl+Shift+S"))
					SaveSceneAs();

				ImGui::EndMenu();
			}
			ImGui::EndMainMenuBar();
		}


		ImGui::Begin("Stats");
		{
			if (m_ActiveScene->GetMainCameraEntity())
			{
				auto&& [cameraComponent, tagComponent] = m_ActiveScene->GetMainCameraEntity().GetComponents<CameraComponent, TagComponent>();
				ImGui::Text("Camera: %s", tagComponent.Tag.c_str());
			}
			else
			{
				ImGui::Text("Camera: %s", "None");
			}

			ImGui::Separator();
			auto stats = Renderer2D::GetStats();
			ImGui::Text("Draw Calls: %llu", stats.DrawCalls);
			ImGui::Text("Quads: %llu", stats.QuadCount);
			ImGui::Text("FrameTime: %.3fms", m_LastFrameTime);
			if (ImGui::Button("Close"))
			{
				Application::Get().Close();
			}
		}
		ImGui::End();

		m_SceneHierarchyPanel.OnImGuiRender();

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

		}

		//Gizmos
		Entity selectedEntity = m_SceneHierarchyPanel.GetSelectedEntity();
		if (selectedEntity && m_GizmoOperation > 0)
		{
			ImGuizmo::SetDrawlist();
			float windowWidth = static_cast<float>(ImGui::GetWindowWidth());
			float windowHeight = static_cast<float>(ImGui::GetWindowHeight());
			ImGuizmo::SetRect(ImGui::GetWindowPos().x, ImGui::GetWindowPos().y, windowWidth, windowHeight);
			;
			const glm::mat4& cameraProjection = m_EditorCamera.GetProjection();
			const glm::mat4& cameraView = m_EditorCamera.GetViewMatrix();

			//Entity
			auto& tc = selectedEntity.GetComponent<TransformComponent>();
			glm::mat4 transform = tc.GetTransform();


			static bool isOrtho = false;
			ImGuizmo::SetOrthographic(isOrtho);

			bool snap = IsGizmoSnapping();
			float snapValues[3] = { m_SnapValue, m_SnapValue, m_SnapValue };

			ImGuizmo::Manipulate(glm::value_ptr(cameraView), glm::value_ptr(cameraProjection),
				static_cast<ImGuizmo::OPERATION>(m_GizmoOperation), ImGuizmo::LOCAL, glm::value_ptr(transform),
				nullptr, snap ? snapValues : nullptr);

			if (ImGuizmo::IsUsing())
			{
				glm::vec3 translation, rotation, scale;
				Math::DecomposeTransform(transform, translation, rotation, scale);

				tc.Translation = translation;
				tc.Rotation = rotation;
				tc.Scale = scale;
			}
		}


		ImGui::End();
		ImGui::PopStyleVar(1);
	}
	void EditorLayer::NewScene()
	{
		m_ActiveScene = CreateRef<Scene>();
		m_SceneHierarchyPanel.SetContext(m_ActiveScene);
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
		std::filesystem::path filepath = FileDialogs::OpenFile("Cine Scene (*.cine)\0*.cine\0");
		if (!filepath.empty())
		{
			m_ActiveScene = CreateRef<Scene>();
			m_SceneHierarchyPanel.SetContext(m_ActiveScene);

			SceneSerializer serializer(m_ActiveScene);
			serializer.Deserialize(filepath);
			m_ActiveScene->OnViewportResize(m_ViewportSize.x, m_ViewportSize.y);
		}
	}

	bool EditorLayer::IsGizmoSnapping() const
	{
		return Input::IsKeyPressed(Key::LeftControl);
	}

	bool EditorLayer::OnKeyPressed(KeyPressedEvent& e)
	{
		if (e.GetRepeatCount() > 0)
		{
			return false;
		}
		bool control = Input::IsKeyPressed(Key::LeftControl) || Input::IsKeyPressed(Key::RightControl);
		bool shift = Input::IsKeyPressed(Key::LeftShift) || Input::IsKeyPressed(Key::RightShift);

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
				OpenScene();
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