
#include "Sandbox2D.hpp"

#include "glash/Core/Timer.hpp"
#include "glash/Debug/Instrumentor.hpp"
#include "glash/Renderer/RenderCommand.hpp"

static int s_Rows = 10;
static int s_Columns = 10;
static float s_QuadSpacing = 0.1f;
static float s_QuadSize = 1.0f;
static glm::vec4 s_ColorStart = { 1.0f, 0.0f, 0.0f, 1.0f };
static glm::vec4 s_ColorEnd = { 0.0f, 0.0f, 1.0f, 1.0f };

void Sandbox2D::OnAttach()
{
	Cine::FrameBufferSpecification spec;
	spec.Width = 1280;
	spec.Height = 720;
	m_FrameBuffer = Cine::FrameBuffer::Create(spec);

	m_CheckerBoardTexture = Cine::Texture2D::Create("resources/textures/checkerboard.png");
	m_FaceTexture = Cine::Texture2D::Create("resources/textures/face.png");
	m_SpriteSheet = Cine::Texture2D::Create("resources/textures/SpriteSheet_Sample.png");
}

void Sandbox2D::OnDetach()
{

}

void Sandbox2D::OnUpdate(Cine::Timestep ts)
{
	CINE_PROFILE_FUNCTION();

	Cine::RenderCommand::Clear();
	m_FrameBuffer->Bind();

	Cine::Renderer2D::ResetStats();

	m_CameraController.OnUpdate(ts);

	float totalWidth = (s_Columns - 1) * s_QuadSpacing + s_Columns * s_QuadSize;
	float totalHeight = (s_Rows - 1) * s_QuadSpacing + s_Rows * s_QuadSize;

	float offsetStartX = -totalWidth / 2.0f + s_QuadSize / 2.0f;
	float offsetStartY = -totalHeight / 2.0f + s_QuadSize / 2.0f;


	Cine::Renderer2D::BeginScene(m_CameraController.GetCamera());
	{
		Cine::Renderer2D::DrawQuad({ 0.0f, 0.0f, 0.0f }, { 20.0f, 20.0f }, m_CheckerBoardTexture, 25.0f);
		CINE_PROFILE_SCOPE("Sending Quads");
		for (int y = 0; y < s_Rows; y++)
		{
			float offsetY = offsetStartY + y * (s_QuadSpacing + s_QuadSize);

			for (int x = 0; x < s_Columns; x++)
			{
				float offsetX = offsetStartX + x * (s_QuadSpacing + s_QuadSize);

				float normalizedOffset = ((offsetX + offsetY) / 2.0f + (totalWidth / 2.0f + totalHeight / 2.0f)) / (totalWidth + totalHeight);
				glm::vec4 color = glm::mix(s_ColorStart, s_ColorEnd, normalizedOffset);
				if((y + x) % 2)
				{
					Cine::Renderer2D::DrawQuad({ offsetX, offsetY, 0.5f }, { s_QuadSize, s_QuadSize }, m_FaceTexture, 1.0f, glm::vec4(color.x + 0.25f, color.y + 0.25f, 1.0f, 1.0f));
				}
				else
				{
					Cine::Renderer2D::DrawQuad({ offsetX, offsetY, 0.1f }, { s_QuadSize, s_QuadSize }, color);
				}
			}
		}
	}
	Cine::Renderer2D::EndScene();

	m_FrameBuffer->Unbind();
}

void Sandbox2D::OnEvent(Cine::Event& event)
{
	m_CameraController.OnEvent(event);
}

void Sandbox2D::OnImGuiRender()
{
	CINE_PROFILE_FUNCTION();

	static bool dockingEnabled = true;

	if (dockingEnabled)
	{
		ImGui::DockSpaceOverViewport(0, ImGui::GetMainViewport());
	}

	ImGui::Begin("Sandbox");
	ImGui::DragInt("Rows", &s_Rows, 1.0f, 1, 100'000'000);
	ImGui::DragInt("Columns", &s_Columns, 1.0f, 1, 100'000'000);
	ImGui::DragFloat("QuadSize size", &s_QuadSize, 0.1f, 0.1f, 100.0f);
	ImGui::DragFloat("Spacing", &s_QuadSpacing, 0.01f, 0.0f, 100.0f);
	ImGui::DragFloat4("Start color", glm::value_ptr(s_ColorStart), 0.01f, 0.0f, 1.0f, "%.2f");
	ImGui::DragFloat4("End color", glm::value_ptr(s_ColorEnd), 0.01f, 0.0f, 1.0f, "%.2f");
	if (ImGui::Checkbox("VSync", &m_VSync))
	{
		s_Application->GetWindow().SetVSync(m_VSync);
	}
	if (ImGui::Checkbox("Docking", &dockingEnabled))
	{
		
	}
	if (ImGui::Button("Reset"))
	{
		s_Rows = 10;
		s_Columns = 10;
		s_QuadSpacing = 0.1f;
		s_QuadSize = 1.0f;
		m_CameraController.Reset();
	}
	uint32_t id = m_FrameBuffer->GetColorAttachmentRendererID();
	ImGui::Image((void*)id, ImVec2(1280, 720), {0, 1}, {1, 0});

	auto& stats = Cine::Renderer2D::GetStats();
	ImGui::Text("Draw Calls: %llu", stats.DrawCalls);
	ImGui::Text("Quads: %llu", stats.QuadCount);

	ImGui::End();
}
