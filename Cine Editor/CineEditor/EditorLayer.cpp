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

			ImGui::Begin("Viewport");
			{
				size_t id = m_FrameBuffer->GetColorAttachmentRendererID();
				ImGui::Image(reinterpret_cast<void*>(id), { 1280, 720 }, { 0, 1 }, { 1, 0 });
			}
			ImGui::End();

			ImGui::Checkbox("Docking", &m_DockingEnabled);
			if (ImGui::Button("Close"))
			{
				Application::Get().Close();
			}
		}

		ImGui::End();
	}
}