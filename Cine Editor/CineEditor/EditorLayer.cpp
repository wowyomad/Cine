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
		Renderer2D::ResetStats();
		m_LastFrameTime = ts.Milleseconds();

		if(m_ViewportHovered && m_ViewportFocused)
		{
			m_CameraController->OnUpdate(ts);
		}

		m_FrameBuffer->Bind();
		Renderer2D::BeginScene(m_CameraController->GetCamera());
		{
			RenderCommand::Clear();
			static float rotation = 0.0f;
			rotation += ts * 15.0f;

			Renderer2D::DrawRotatedQuad({ 0.0f, 0.0f, 0.0f }, { 10.0f, 10.0f }, glm::radians(rotation), m_CheckerboardTexture, 25.0f);
			Renderer2D::DrawRotatedQuad({ 0.0f, 0.0f, 0.0f }, { 10.0f, 10.0f }, glm::radians(-rotation), m_CheckerboardTexture, 25.0f);

			Renderer2D::DrawRotatedQuad({ 0.0f, 10.0f, 0.0f }, { 10.0f, 10.0f }, glm::radians(rotation), m_CheckerboardTexture, 25.0f);
			Renderer2D::DrawRotatedQuad({ 0.0f, 10.0f, 0.0f }, { 10.0f, 10.0f }, glm::radians(-rotation), m_CheckerboardTexture, 25.0f);

			Renderer2D::DrawRotatedQuad({ 10.0f, 0.0f, 0.0f }, { 10.0f, 10.0f }, glm::radians(rotation), m_CheckerboardTexture, 25.0f);
			Renderer2D::DrawRotatedQuad({ 10.0f, 0.0f, 0.0f }, { 10.0f, 10.0f }, glm::radians(-rotation), m_CheckerboardTexture, 25.0f);

			Renderer2D::DrawRotatedQuad({ 10.0f, 10.0f, 0.0f }, { 10.0f, 10.0f }, glm::radians(rotation), m_CheckerboardTexture, 25.0f);
			Renderer2D::DrawRotatedQuad({ 10.0f, 10.0f, 0.0f }, { 10.0f, 10.0f }, glm::radians(-rotation), m_CheckerboardTexture, 25.0f);

			Renderer2D::DrawRotatedQuad({ 10.0f, -10.0f, 0.0f }, { 10.0f, 10.0f }, glm::radians(rotation), m_CheckerboardTexture, 25.0f);
			Renderer2D::DrawRotatedQuad({ 10.0f, -10.0f, 0.0f }, { 10.0f, 10.0f }, glm::radians(-rotation), m_CheckerboardTexture, 25.0f);

			Renderer2D::DrawRotatedQuad({ 0.0f, -10.0f, 0.0f }, { 10.0f, 10.0f }, glm::radians(rotation), m_CheckerboardTexture, 25.0f);
			Renderer2D::DrawRotatedQuad({ 0.0f, -10.0f, 0.0f }, { 10.0f, 10.0f }, glm::radians(-rotation), m_CheckerboardTexture, 25.0f);

			Renderer2D::DrawRotatedQuad({ -10.0f, 0.0f, 0.0f }, { 10.0f, 10.0f }, glm::radians(rotation), m_CheckerboardTexture, 25.0f);
			Renderer2D::DrawRotatedQuad({ -10.0f, 0.0f, 0.0f }, { 10.0f, 10.0f }, glm::radians(-rotation), m_CheckerboardTexture, 25.0f);

			Renderer2D::DrawRotatedQuad({ -10.0f, 10.0f, 0.0f }, { 10.0f, 10.0f }, glm::radians(rotation), m_CheckerboardTexture, 25.0f);
			Renderer2D::DrawRotatedQuad({ -10.0f, 10.0f, 0.0f }, { 10.0f, 10.0f }, glm::radians(-rotation), m_CheckerboardTexture, 25.0f);

			Renderer2D::DrawRotatedQuad({ -10.0f, -10.0f, 0.0f }, { 10.0f, 10.0f }, glm::radians(rotation), m_CheckerboardTexture, 25.0f);
			Renderer2D::DrawRotatedQuad({ -10.0f, -10.0f, 0.0f }, { 10.0f, 10.0f }, glm::radians(-rotation), m_CheckerboardTexture, 25.0f);

		}
		Renderer2D::EndScene();
		m_FrameBuffer->Unbind();
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
	}
}