#include "glash/glash_pch.hpp"
#include "EditorLayer.hpp"

namespace Cine
{
	void EditorLayer::OnAttach()
	{
		FrameBufferSpecification spec;
		spec.Width = 1280;
		spec.Height = 720;
		m_FrameBuffer = FrameBuffer::Create(spec);
		m_CameraController = CreateRef<OrthograhpicCameraController>(1.7778f);
	
		m_CheckerboardTexture = Texture2D::Create("resources/textures/checkerboard.png");
	}

	void EditorLayer::OnUpdate(Timestep ts)
	{
		m_CameraController->OnUpdate(ts);

		m_FrameBuffer->Bind();
		Renderer2D::BeginScene(m_CameraController->GetCamera());
		{
			RenderCommand::Clear();

			Renderer2D::DrawQuad({ 0.0f, 0.0f, 0.0f }, { 10.0f, 10.0f }, m_CheckerboardTexture, 25.0f);
		}
		Renderer2D::EndScene();
		m_FrameBuffer->Unbind();
	}

	void EditorLayer::OnEvent(Event& event)
	{
		m_CameraController->OnEvent(event);
	}

	void EditorLayer::OnImGuiRender()
	{
		if (m_DockingEnabled)
		{
			ImGui::DockSpaceOverViewport(0, ImGui::GetMainViewport());
		}

		ImGui::Begin("Editor");
		{
			ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, { 0.0f, 0.0f });
			ImGui::Begin("Viewport");
			{
				ImVec2 viewportPanelSize = ImGui::GetContentRegionAvail();
				glm::vec2 viewportSize = { viewportPanelSize.x, viewportPanelSize.y };
				if (viewportSize != m_ViewportSize)
				{
					m_ViewportSize = viewportSize;
					m_FrameBuffer->Resize(static_cast<uint32_t>(m_ViewportSize.x), static_cast<uint32_t>(m_ViewportSize.y));
					m_CameraController->OnResize(m_ViewportSize.x, m_ViewportSize.y);
				}
				size_t id = m_FrameBuffer->GetColorAttachmentRendererID();
				ImGui::Image(reinterpret_cast<void*>(id), { m_ViewportSize.x, m_ViewportSize.y }, { 0, 1 }, { 1, 0 });

			}
			ImGui::End();
			ImGui::PopStyleVar(1);

			ImGui::Checkbox("Docking", &m_DockingEnabled);
			if (ImGui::Button("Close"))
			{
				Application::Get().Close();
			}
		}

		ImGui::End();
	}
}