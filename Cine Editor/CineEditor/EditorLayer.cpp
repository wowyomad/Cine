#include "glash/glash_pch.hpp"
#include "EditorLayer.hpp"

namespace Cine
{
	void EditorLayer::OnAttach()
	{
		FramebufferSpecification spec;
		spec.Width = 1280;
		spec.Height = 720;
		m_Framebuffer = FrameBuffer::Create(spec);

		m_CameraController = CreateRef<OrthograhpicCameraController>(1.7778f);
	
		m_CheckerboardTexture = Texture2D::Create("resources/textures/checkerboard.png");

		m_ActiveScene = CreateRef<Scene>(); 

		m_FirstCamera = m_ActiveScene->CreateEntity("First Camera");
		m_FirstCamera.AddComponent<CameraComponent>();
		m_ActiveScene->SetMainCamera(m_FirstCamera);

		m_SecondCamera = m_ActiveScene->CreateEntity("Second Camera");
		m_SecondCamera.AddComponent<CameraComponent>();
		m_ActiveScene->SetMainCamera(m_SecondCamera);

		m_SquareEntity = m_ActiveScene->CreateEntity("Square");
		m_SquareEntity.AddComponent<SpriteRendererComponent>(glm::vec4(1.0f, 0.75f, 0.75f, 1.0f));		 
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

		DrawViewport();

		ImGui::Begin("Editor");
		{
			auto& sprite = m_SquareEntity.GetComponent<SpriteRendererComponent>();
			ImGui::ColorEdit4("Square Color", glm::value_ptr(sprite.Color));


			auto mainCamera = m_ActiveScene->GetMainCamera();
			if (ImGui::Button("Switch Camera"))
			{
				if (mainCamera == m_FirstCamera)
				{
					m_ActiveScene->SetMainCamera(m_SecondCamera);
				}
				else
				{
					m_ActiveScene->SetMainCamera(m_FirstCamera);
				}
			}

			auto&& [cameraComponent, tagComponent] = m_ActiveScene->GetMainCamera().GetComponents<CameraComponent, TagComponent>();
			ImGui::Text("Camera: %s", tagComponent.Tag.c_str());
			
			float orthoSize = cameraComponent.Camera.GetOrthographicSize();
			if (ImGui::DragFloat("Camera Ortho", &orthoSize))
			{
				cameraComponent.Camera.SetOrthographicSize(orthoSize);
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