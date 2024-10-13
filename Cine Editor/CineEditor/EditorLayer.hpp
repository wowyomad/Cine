#include "glash/Cine.hpp"

namespace Cine
{
	class EditorLayer : public Layer
	{
	public:
		EditorLayer() : Layer("Cine Editor") {}

		void OnAttach() override;
		void OnUpdate(Timestep ts) override;
		void OnEvent(Event& event) override;
		void OnImGuiRender() override;

	private:
		
		bool m_DockingEnabled = true;
		Ref<OrthograhpicCameraController> m_CameraController;

		Ref<FrameBuffer> m_FrameBuffer;
		Ref<Texture2D> m_CheckerboardTexture;

		glm::vec2 m_ViewportSize = { 1280.0f, 720.0f };
		bool m_ViewportFocused = false;
		bool m_ViewportHovered = false;

		float m_LastFrameTime = 0.0f;

	private:
		void DrawViewport();
	};
}
