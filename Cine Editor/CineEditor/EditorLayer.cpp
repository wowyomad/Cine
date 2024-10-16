#include "glash/glash_pch.hpp"
#include "EditorLayer.hpp"

#include "Scene/ScriptableEntity.hpp"
#include "Scene/SceneSerializer.hpp"
#include "glash/Utils/PlatformUtils.hpp"

#include <ImGuizmo.h>

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


		m_ActiveScene = CreateRef<Scene>();
		s_Scene = m_ActiveScene.get();

		m_HierarchyPanel.SetContext(m_ActiveScene);
	}

	void EditorLayer::OnDetach()
	{

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

		if (m_ViewportHovered && m_ViewportFocused)
		{

		}

		m_Framebuffer->Bind();
		{
			m_ActiveScene->OnUpdate(ts);
		}
		m_Framebuffer->Unbind();
	}

	void EditorLayer::OnEvent(Event& event)
	{
		if (m_ViewportHovered && m_ViewportFocused)
		{

		}

		EventDispatcher dispatcher(event);
		dispatcher.Dispatch<KeyPressedEvent>(CINE_BIND_EVENT_FN(EditorLayer::OnKeyPressed));
	}

	void EditorLayer::OnImGuiRender()
	{
		if (m_DockingEnabled)
		{
			ImGui::DockSpaceOverViewport(0, ImGui::GetMainViewport());
		}

		if (ImGui::BeginMainMenuBar())
		{
			if (ImGui::BeginMenu("File"))
			{
				if (ImGui::MenuItem("New", "Ctrl+N"))
					NewScene();

				if (ImGui::MenuItem("Open...", "Ctrl+O"))
					OpenScene();

				if (ImGui::MenuItem("Save As...", "Ctrl+Shift+S"))
					SaveSceneAs();

				ImGui::EndMenu();
			}
			ImGui::EndMainMenuBar();
		}


		ImGui::Begin("Stats");
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

		m_HierarchyPanel.OnImGuiRender();

		DrawViewport();

	}
	bool EditorLayer::OnKeyPressed(KeyPressedEvent& e)
	{
		if (e.GetRepeatCount() > 0)
		{
			return false;
		}
		bool control = Input::IsKeyPressed(Key::LeftControl) || Input::IsKeyPressed(Key::RightControl);
		bool shift = Input::IsKeyPressed(Key::LeftShift) || Input::IsKeyPressed(Key::RightShift);
		switch (e.GetKeyCode())
		{
		case Key::N:
		{
			if(control)
				NewScene();
		} break;
		case Key::O:
		{
			if(control)
				OpenScene();
		} break;
		case Key::S:
		{
			if(control && shift)
				SaveSceneAs();
		} break;
		}
		return false;
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
	void EditorLayer::NewScene()
	{
		m_ActiveScene = CreateRef<Scene>();
		m_HierarchyPanel.SetContext(m_ActiveScene);
		//m_ActiveScene->OnViewportResize(m_ViewportSize.x, m_ViewportSize.y);
	}
	void EditorLayer::SaveSceneAs()
	{
		std::filesystem::path filepath = FileDialogs::SaveFile("Cine Scene (*.cine)\0*.cine\0");
		if (!filepath.empty())
		{
			SceneSerializer serializer(m_ActiveScene);
			serializer.Serialize(filepath);
		}
	}
	void EditorLayer::OpenScene()
	{
		std::filesystem::path filepath = FileDialogs::OpenFile("Cine Scene (*.cine)\0*.cine\0");
		if (!filepath.empty())
		{
			m_ActiveScene = CreateRef<Scene>();
			m_HierarchyPanel.SetContext(m_ActiveScene);
			//m_ActiveScene->OnViewportResize(m_ViewportSize.x, m_ViewportSize.y);

			SceneSerializer serializer(m_ActiveScene);
			serializer.Deserialize(filepath);
		}
	}
}