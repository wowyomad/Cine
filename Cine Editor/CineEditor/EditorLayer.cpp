#include "glash/glash_pch.hpp"
#include "EditorLayer.hpp"

#include "Scene/Components.hpp"
#include "Scene/SceneSerializer.hpp"
#include "Scene/ComponentSerializer.hpp"
#include "glash/Utils/PlatformUtils.hpp"
#include "glash/Core/Timer.hpp"
#include "glash/Math/Math.hpp"


#include <ImGuizmo.h>

static Cine::Scene* s_Scene = nullptr;

enum PlayerAnimation
{
	Idle,
	MoveUp,
	MoveRight,
	MoveLeft,
	MoveDown,
};

namespace Cine
{
	class RotationScript : public NativeScript
	{
	public:
		RotationScript() = default;
		RotationScript(float rotationSpeed)
			: m_RotationSpeed(rotationSpeed) {}

		void OnCreate() override
		{
			m_LocalTransform = &GetComponent<TransformComponent>();
		}

		void OnUpdate(Timestep ts) override
		{
			if (m_LocalTransform)
			{
				m_LocalTransform->Rotation.z += glm::radians(m_RotationSpeed) * ts;
			}
		}

	private:
		TransformComponent* m_LocalTransform;
		float m_RotationSpeed = 90.0f;

		BEGIN_SERIALIZE(RotationScript)
			SERIALIZE_FIELD(m_RotationSpeed)
			END_SERIALIZE()
	};

	class ColorScript : public NativeScript
	{
	public:
		void OnCreate() override
		{
			m_SpriteComponent = TryGetComponent<SpriteComponent>();
			m_Timer.Start();
		}

		void OnUpdate(Timestep ts) override
		{
			if (!m_SpriteComponent)
			{
				return;
			}

			m_Timer.OnUpdate(ts);

			m_Time = m_Timer.GetElapsed();
			m_SpriteComponent->Color.r = 0.5f * sin(m_Time) + 0.5f;
			m_SpriteComponent->Color.g = 0.5f * sin(m_Time + 2.0f) + 0.5f;
			m_SpriteComponent->Color.b = 0.5f * sin(m_Time + 4.0f) + 0.5f;
		}
	public:

		BEGIN_SERIALIZE(ColorScript)
			SERIALIZE_FIELD(m_String)
			SERIALIZE_FIELD(m_Time)
			END_SERIALIZE()

	private:
		SpriteComponent* m_SpriteComponent = nullptr;
		Timer m_Timer;
		float m_Time;
		std::string m_String = "String from Color Script";

	};


	class ControllerScript : public NativeScript
	{
	public:
		void OnCreate() override
		{
			m_Anim = TryGetComponent<SpriteAnimationComponent>();
			m_Transform = TryGetComponent<TransformComponent>();
		}

		void OnDestroy() override
		{

		}

		void OnUpdate(Timestep ts) override
		{
			if (!m_Anim || !m_Transform)
			{
				return;
			}

			float speed = 5.0f;
			glm::vec3 direction(0.0f);

			if (Input::IsKeyPressed(Key::D))
			{
				direction.x += 1.0f;
				m_Anim->State.DesiredAnimation = PlayerAnimation::MoveRight;
			}
			if (Input::IsKeyPressed(Key::A))
			{
				direction.x -= 1.0f;
				m_Anim->State.DesiredAnimation = PlayerAnimation::MoveLeft;
			}
			if (Input::IsKeyPressed(Key::W))
			{
				direction.y += 1.0f;
				m_Anim->State.DesiredAnimation = PlayerAnimation::MoveUp;
			}
			if (Input::IsKeyPressed(Key::S))
			{
				direction.y -= 1.0f;
				m_Anim->State.DesiredAnimation = PlayerAnimation::MoveDown;
			}

			if (glm::length(direction) == 0.0f)
			{
				m_Anim->State.DesiredAnimation = PlayerAnimation::Idle;
			}

			m_Transform->Translation += speed * ts * direction;

		}

		BEGIN_SERIALIZE(ControllerScript)

			END_SERIALIZE()

	private:
		SpriteAnimationComponent* m_Anim;
		TransformComponent* m_Transform;
	};

	void EditorLayer::OnAttach()
	{
		m_IsRuntime = true;

		m_ActiveScene = CreateRef<Scene>();
		s_Scene = m_ActiveScene.get();

		FramebufferSpecification spec;
		spec.Width = 1280;
		spec.Height = 720;
		m_Framebuffer = FrameBuffer::Create(spec);

		m_ActiveScene = CreateRef<Scene>();
		s_Scene = m_ActiveScene.get();

		m_SceneHierarchyPanel.SetContext(m_ActiveScene);

		m_EditorCamera = EditorCamera(45.0f, 16.0f / 9.0f, 0.01f, 1000.0f);

		s_Scene->RegisterComponent<ControllerScript>();
		s_Scene->RegisterComponent<RotationScript>();
		s_Scene->RegisterComponent<ColorScript>();

		auto woman = m_ActiveScene->CreateEntity("Woman");
		woman.AddComponents<ControllerScript, SpriteComponent, SpriteRendererComponent>();
		auto&& sheet = woman.AddComponent<SpriteSheetComponent>();
		auto&& anim = woman.AddComponent<SpriteAnimationComponent>();
		woman.GetComponent<SpriteRendererComponent>().UseSprite = true;
		woman.AddComponent<RotationScript>(-180.0f);
		woman.AddComponent<ColorScript>();
		woman.GetComponent<TransformComponent>().Translation.z += 0.1f;

		auto square = m_ActiveScene->CreateEntity("Square");
		square.AddComponents<SpriteRendererComponent, SpriteSheetComponent, SpriteComponent, RotationScript>();
		square.GetComponent<SpriteComponent>().Color = { 0.2f, 0.8f, 0.2f, 1.0f };
		square.AddChild(woman);



		sheet = AssetManager::LoadSpriteSheet("Woman", "Textures/Woman_Sheet.png", false);

		SpriteAnimationComponent::Animation animation;
		animation.Loop = true;
		animation.Duration = 0.5f;

		animation.SpriteFrames = { 0, 1, 2, 3, 4, 5, 6, 7 };
		anim.Animations[PlayerAnimation::MoveUp] = animation;

		animation.SpriteFrames = { 8, 9, 10, 11, 12, 13, 14, 15 };
		anim.Animations[PlayerAnimation::MoveRight] = animation;

		animation.SpriteFrames = { 16, 17, 18, 19, 20, 21, 22, 23 };
		anim.Animations[PlayerAnimation::MoveLeft] = animation;

		animation.SpriteFrames = { 24, 25, 26, 27, 28, 29, 30, 31 };
		anim.Animations[PlayerAnimation::MoveDown] = animation;

		anim.State.DefaultAnimation = -1;

		animation.Loop = true;
		animation.SpriteFrames = { 0 };
		anim.Animations[PlayerAnimation::Idle] = animation;

		auto camera = s_Scene->CreateEntity("Camera");
		camera.AddComponent<CameraComponent>();
		s_Scene->SetMainCamera(camera);
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
			if (m_IsRuntime)
				m_ActiveScene->OnUpdateRuntime(ts);
			else
				m_ActiveScene->OnUpdateEditor(ts, m_EditorCamera);
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

		}

		//Gizmos
		if (m_IsRuntime)
		{
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
				glm::mat4 transform = tc.GetLocalTransform();


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
		if (m_IsRuntime) return false;

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

	struct HealthComponent
	{
		float value;
		glm::vec2 vec2;
		std::vector<std::string> values;

		BEGIN_SERIALIZE(HealthComponent)
			SERIALIZE_FIELD(value)
			SERIALIZE_FIELD(vec2)
			SERIALIZE_FIELD(values)
			END_SERIALIZE()
	};


	class PlayerScript
	{
	public:
		float Speed;
		std::string Name;
		HealthComponent Health;

	public:
		void OnStart()
		{

		}

		void Set()
		{
			Speed = 15.0f;
			Name = "Alexei";
			Health = { 15, {25.0, 60.0f} };
			Health.values = { "Biba", "Boba", "Dima" };
		}

		BEGIN_SERIALIZE(PlayerScript)
			SERIALIZE_FIELD(Speed)
			SERIALIZE_FIELD(Name)
			SERIALIZE_FIELD(Health)
			END_SERIALIZE()
	};

	void EditorLayer::SetupCustom()
	{
		PlayerScript p;
		p.Set();

		YAML::Node node = Serialize(p);
		std::string serializedString = YAML::Dump(node);
		std::cout << "Serialized String: " << std::endl << serializedString << std::endl;

		PlayerScript deserializedPlayer;
		Deserialize(deserializedPlayer, node);

		std::cout << "Deserialized Player Name: " << deserializedPlayer.Name << std::endl;
		std::cout << "Deserialized Player Speed: " << deserializedPlayer.Speed << std::endl;
		std::cout << "Deserialized Health Value: " << deserializedPlayer.Health.value << std::endl;
		std::cout << "Deserialized Health Vec2: " << deserializedPlayer.Health.vec2.x << ", " << deserializedPlayer.Health.vec2.y << std::endl;
		int i = 0;
		for (auto& value : deserializedPlayer.Health.values)
		{
			std::cout << "Deserialized value " << ++i << ": " << value << std::endl;

		}
	}
}