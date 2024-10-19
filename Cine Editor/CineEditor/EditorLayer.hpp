#include "glash/Cine.hpp"

#include <ImGuizmo.h>

#include "Panels/SceneHierarchyPanel.hpp"
#include "glash/Renderer/Texture.hpp"
#include "glash/Scene/AssetManager.hpp"

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

		void SetupCustom();

	private:

		Ref<FrameBuffer> m_Framebuffer;
		Ref<Scene> m_ActiveScene;
		SceneHierarchyPanel m_SceneHierarchyPanel;
		EditorCamera m_EditorCamera;

		int32_t m_GizmoOperation = -1;
		float m_SnapScale = 0.1f;
		float m_SnapTranslation = 0.5f;
		float m_SnapRotation = 5.0f;
		float m_SnapValue = 0.0f;

		glm::vec2 m_ViewportSize = { 0.0f, 0.0f };
		bool m_ViewportFocused = true;
		bool m_ViewportHovered = true;

		AssetManager m_AssetManager;
		std::filesystem::path m_ScenePath = "Assets/Scenes/Example.cine";

		float m_LastFrametime = 0.0f;

	private:
		bool OnKeyPressed(KeyPressedEvent& e);
		void DrawViewport();

		void NewScene();
		void SaveSceneAs();
		void OpenScene();

		bool IsGizmoSnapping() const;
	};
}
