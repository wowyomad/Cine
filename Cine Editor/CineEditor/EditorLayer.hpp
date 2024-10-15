#include "glash/Cine.hpp"

#include "Panels/SceneHierarchyPanel.hpp"

namespace Cine
{
	class EditorLayer : public Layer
	{
	public:
		EditorLayer() : Layer("Cine Editor") {}
		

		void OnAttach() override;
		void OnDetach() override;
		void OnUpdate(Timestep ts) override;
		void OnEvent(Event& event) override;
		void OnImGuiRender() override;

	private:
		bool m_DockingEnabled = true;

		Ref<FrameBuffer> m_Framebuffer;


		Ref<Scene> m_ActiveScene;
		SceneHierarchyPanel m_HierarchyPanel;

		glm::vec2 m_ViewportSize = { 0.0f, 0.0f };
		bool m_ViewportFocused = true;
		bool m_ViewportHovered = true;

		float m_LastFrameTime = 0.0f;

		std::filesystem::path m_ScenePath = "Assets/Scenes/Example.cine";

	private:
		void DrawViewport();
	};
}
