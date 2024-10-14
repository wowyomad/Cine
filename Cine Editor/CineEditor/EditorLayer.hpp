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

		Entity m_FirstCamera;
		Entity m_SecondCamera;

		Ref<FrameBuffer> m_Framebuffer;
		Ref<Texture2D> m_CheckerboardTexture;

		Entity m_SquareEntity;

		Ref<Scene> m_ActiveScene;

		glm::vec2 m_ViewportSize = { 0.0f, 0.0f };
		bool m_ViewportFocused = true;
		bool m_ViewportHovered = true;

		float m_LastFrameTime = 0.0f;

	private:
		void DrawViewport();
	};
}
