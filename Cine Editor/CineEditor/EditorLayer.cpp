#include "glash/glash_pch.hpp"
#include "EditorLayer.hpp"

#include "Scene/ScriptableEntity.hpp"

static Cine::Scene* s_Scene = nullptr;

namespace Cine
{
	class CameraControllerScript : public ScriptableEntity
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
			if (GetEntity() == s_Scene->GetMainCamera())
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
		}
	};

	void EditorLayer::OnAttach()
	{

		FramebufferSpecification spec;
		spec.Width = 1280;
		spec.Height = 720;
		m_Framebuffer = FrameBuffer::Create(spec);

		m_CameraController = CreateRef<OrthograhpicCameraController>(1.7778f);

		m_ActiveScene = CreateRef<Scene>(); 
		s_Scene = m_ActiveScene.get();

		m_FirstCamera = m_ActiveScene->CreateEntity("First Camera");
		m_FirstCamera.AddComponent<CameraComponent>();
		m_FirstCamera.AddComponent<NativeScriptComponent>().Bind<CameraControllerScript>();

		m_SecondCamera = m_ActiveScene->CreateEntity("Second Camera");
		m_SecondCamera.AddComponent<CameraComponent>();
		m_SecondCamera.AddComponent<NativeScriptComponent>().Bind<CameraControllerScript>();

		m_ActiveScene->SetMainCamera(m_FirstCamera); 

		m_SquareEntity = m_ActiveScene->CreateEntity("Square");
		m_FaceTexture = Texture2D::Create("resources/textures/face.png");
		m_SquareEntity.AddComponent<SpriteRendererComponent>(glm::vec4(1.0f, 0.75f, 0.75f, 1.0f));	

		m_HierarchyPanel.SetContext(m_ActiveScene);
	}

	void EditorLayer::OnUpdate(Timestep ts)
	{
		if (FramebufferSpecification spec = m_Framebuffer->GetSpecification();
			m_ViewportSize.x > 0.0f && m_ViewportSize.y > 0.0f &&
			(spec.Width != m_ViewportSize.x || spec.Height != m_ViewportSize.y))
		{
			m_Framebuffer->Resize((uint32_t)m_ViewportSize.x, (uint32_t)m_ViewportSize.y);
			m_ActiveScene->OnViewportResize((uint32_t)m_ViewportSize.x, (uint32_t)m_ViewportSize.y);
		}

		Renderer2D::ResetStats();
		m_LastFrameTime = ts.Milleseconds();

		if(m_ViewportHovered && m_ViewportFocused)
		{
			m_CameraController->OnUpdate(ts);
		}

		m_Framebuffer->Bind();
		{
			m_ActiveScene->OnUpdate(ts);
		}
		m_Framebuffer->Unbind();
	}

	void EditorLayer::OnEvent(Event& event)
	{
		if(m_ViewportHovered && m_ViewportFocused)
		{
			m_CameraController->OnEvent(event);
		}
	}

	void EditorLayer::OnImGuiRender()
	{
		if (m_DockingEnabled)
		{
			ImGui::DockSpaceOverViewport(0, ImGui::GetMainViewport());
		}

		m_HierarchyPanel.OnImGuiRender();

		DrawViewport();

		ImGui::Begin("Editor");
		{
			
			if (m_ActiveScene->GetMainCamera())
			{
				auto&& [cameraComponent, tagComponent] = m_ActiveScene->GetMainCamera().GetComponents<CameraComponent, TagComponent>();
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
			ImGui::Checkbox("Docking", &m_DockingEnabled);
			if (ImGui::Button("Close"))
			{
				Application::Get().Close();
			}
		}

		ImGui::End();
	}
	void EditorLayer::DrawViewport()
	{
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, { 0.0f, 0.0f });
		ImGui::Begin("Viewport");
		{
			m_ViewportFocused = ImGui::IsWindowFocused();
			m_ViewportHovered = ImGui::IsWindowHovered();
			Application::Get().GetImGuiLayer()->SetBlockEvents(!m_ViewportFocused || !m_ViewportHovered);

			ImVec2 viewportPanelSize = ImGui::GetContentRegionAvail();
			m_ViewportSize = { viewportPanelSize.x, viewportPanelSize.y };
				
			size_t id = m_Framebuffer->GetColorAttachmentRendererID();
			ImGui::Image(reinterpret_cast<void*>(id), { m_ViewportSize.x, m_ViewportSize.y }, { 0, 1 }, { 1, 0 });

		}
		ImGui::End();
		ImGui::PopStyleVar(1);
	}
}