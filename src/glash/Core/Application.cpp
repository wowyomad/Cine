#include "glash/glash_pch.hpp"
#include "glash/Core/Application.hpp"

#include "glash/Core/Log.hpp"

#include "glash/Renderer/RenderCommand.hpp"
#include "glash/Renderer/Renderer.hpp"

#include "glash/events/KeyEvent.hpp"
#include "glash/events/ApplicationEvent.hpp"
#include "glash/events/MouseEvent.hpp"

#include "glash/Core/KeyCodes.hpp"

#include "glash/ImGui/ImGuiLayer.hpp"

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/string_cast.hpp>

namespace glash
{
	Application* Application::s_Instance = nullptr;

	Application& Application::Get()
	{
		return *s_Instance;
	}

	GLASH_WINDOW_CLASS& Application::GetWindow()
	{
		return *m_Window;
	}

	Application::Application(int dummy)
		: m_Running(false),
		m_Camera(-1.0f, 1.0f, -1.0f, 1.0f)
	{
		GLASH_LOG_TRACE(BUILD_STR);
		(void)dummy; //get rid of unused parameter warning

		GLASH_CORE_ASSERT(s_Instance == nullptr, "Application should be singleton");
		s_Instance = this;

		m_Window = std::unique_ptr<GLASH_WINDOW_CLASS>(Window::Create());
		m_Window->SetEventCallback(GLASH_BIND_EVENT_FN(Application::OnEvent));

		Renderer::Init();
		RenderCommand::SetClearColor({ 0.15, 0.15, 0.15, 0.15 });

		float ratio = (float)m_Window->GetWidth() / (float)m_Window->GetHeight();
		m_Camera.SetProjection(-ratio, ratio, -1.0, 1.0f);

		m_ImGuiLayer = new ImGuiLayer();
		PushOverlay(m_ImGuiLayer);

		float verticesSquare[] = {
			-0.75f, -0.75f, 0.0f,		0.8f, 0.2f, 0.3f, 1.0,
			 0.75f, -0.75f, 0.0f,		0.2f, 0.8f, 0.3f, 1.0,
			 0.75f,  0.75f, 0.0f,		0.2f, 0.3f, 0.8f, 1.0,
			-0.75f,  0.75f, 0.0f,		0.2f, 0.8f, 0.3f, 1.0
		};

		float verticesTriangle[] = {
			-0.5f * cosf(3.1415 / 6.0f),		-0.5f * sinf(3.1415 / 6.0f),		0.0f,		1.0f, 1.0f, 1.0f, 0.25f,
			 0.5f * cosf(3.1415 / 6.0f),		-0.5f * sinf(3.1415	 / 6.0f),	0.0f,		1.0f, 1.0f, 1.0f, 0.25f,
			 0.0f,								 0.5f,								0.0f,		1.0f, 1.0f, 1.0f, 0.25f,
		};
		unsigned int indicesSquare[] = {
			0, 1, 2,
			2, 3, 0
		};
		unsigned int indicesTriangle[] = {
			0, 1, 2,
		};

		uint32_t vertexBuffer;
		m_VertexArrayTriangle = VertexArray::Create();
		m_VertexArraySquare = VertexArray::Create();

		auto SquareVertexBuffer = VertexBuffer::Create(verticesSquare, sizeof(verticesSquare));
		auto SquareIndexBuffer = IndexBuffer::Create(indicesSquare, sizeof(indicesSquare) / sizeof(float));

		auto TriangleVertexBuffer = VertexBuffer::Create(verticesTriangle, sizeof(verticesTriangle));
		auto TriangleIndexBuffer = IndexBuffer::Create(indicesTriangle, sizeof(indicesTriangle) / sizeof(float));

		BufferLayout SquareLayout = {
			{ ShaderDataType::Float3, "a_Position" },
			{ ShaderDataType::Float4, "a_Color" }
		};
		SquareVertexBuffer->SetLayout(SquareLayout);
	
		BufferLayout TriangleLayout = {
			{ ShaderDataType::Float3, "a_Position" },
			{ ShaderDataType::Float4, "a_Color" }
		};

		SquareVertexBuffer->SetLayout(SquareLayout);
		TriangleVertexBuffer->SetLayout(TriangleLayout);

		m_VertexArraySquare->AddVertexBuffer(SquareVertexBuffer);
		m_VertexArraySquare->SetIndexBuffer(SquareIndexBuffer);
		m_VertexArrayTriangle->AddVertexBuffer(TriangleVertexBuffer);
		m_VertexArrayTriangle->SetIndexBuffer(TriangleIndexBuffer);

		m_Shader = Shader::Create("resources/shaders/shader.shader");
	}

	void Application::OnEvent(Event& event)
	{
		EventDispatcher dispatcher(event);

		if (event.GetEventType() == EventType::MouseScrolled)
		{

		}

		dispatcher.Dispatch<MouseScrolledEvent>([&](MouseScrolledEvent& event) 
			{ 
				scale -= 0.05f * event.GetVertical();
				
				return false;
			});

		for (auto it = m_LayerStack.rbegin(); it != m_LayerStack.rend(); ++it)
		{
			if (event.IsHandled())
			{
				break;
			}
			(*it)->OnEvent(event);
		}

		dispatcher.Dispatch<WindowCloseEvent>(GLASH_BIND_EVENT_FN(Application::OnWindowCloseEvent));
		dispatcher.Dispatch<WindowResizeEvent>(GLASH_BIND_EVENT_FN(Application::OnWindowResizeEvent));


	}

	void Application::PushLayer(Layer* layer)
	{
		m_LayerStack.PushLayer(layer);
		layer->OnAttach();
	}

	void Application::PushOverlay(Layer* overlay)
	{
		m_LayerStack.PushLayer(overlay);
		overlay->OnAttach();
	}

	bool Application::OnWindowCloseEvent(WindowCloseEvent& event)
	{
		m_Running = false;

		return false;
	}

	bool Application::OnWindowResizeEvent(WindowResizeEvent& event)
	{
		float ratio = (float)event.GetWidth() / (float)event.GetHeight();
		m_Camera.SetProjection(scale * -ratio, scale * ratio, scale * -1.0, scale * 1.0f);
		return false;
	}

	void Application::Run()
	{
		m_Running = true;
		while (m_Running)
		{
			auto cameraPositoin = m_Camera.GetPosition();
			float speed = 0.01;
			glm::vec3 translation(0.0f);
			static float rotation = 0.0f;
			static glm::vec4 color(1.0f);
			if (Input::IsKeyPressed(Key::A))
			{
				translation.x -= 1;
			}
			if (Input::IsKeyPressed(Key::D))
			{
				translation.x += 1;
			}
			if (Input::IsKeyPressed(Key::W))
			{
				translation.y += 1;
			}
			if (Input::IsKeyPressed(Key::S))
			{
				translation.y -= 1;
			}
			rotation += 0.5f;

			if (glm::dot(translation, translation) > 0.0)
			{
				translation = glm::normalize(translation) * speed;
			}
			m_Camera.SetPosition(cameraPositoin + -translation);

			float ratio = (float)m_Window->GetWidth() / (float)m_Window->GetHeight();
			m_Camera.SetProjection(scale * -ratio, scale * ratio, scale * -1.0, scale * 1.0f);

			glm::mat4 transform = glm::rotate(glm::mat4(1.0f), glm::radians(rotation), glm::vec3(0.0f, 0.0f, 1.0f));

			Renderer::BeginScene(m_Camera);

			m_Shader->Bind();

			m_Shader->SetInt("u_UseUniformColor", 0);
			Renderer::Submit(m_Shader, m_VertexArraySquare);

			m_Shader->SetInt("u_UseUniformColor", 1);
			m_Shader->SetFloat4("u_Color", color);
			Renderer::Submit(m_Shader, m_VertexArrayTriangle, transform);

			Renderer::EndScene();

			for (Layer* layer : m_LayerStack)
			{
				layer->OnUpdate();
			}

			m_ImGuiLayer->Begin();
			for (Layer* layer : m_LayerStack)
			{
				layer->OnImGuiRender();
			}
			ImGui::Begin("Debug");
			ImGui::SliderFloat4("Color", reinterpret_cast<float*>(&color), 0.0f, 1.0f, "%.2f");
			ImGui::End();
			m_ImGuiLayer->End();

			m_Window->OnUpdate();
		}
	}

	ImGuiContext* Application::GetImGuiContext() const
	{
		return ImGui::GetCurrentContext();
	}

	void Application::GetAllocatorFunctions(ImGuiMemAllocFunc* p_alloc_func, ImGuiMemFreeFunc* p_free_func, void** p_user_data) const
	{
		ImGui::GetAllocatorFunctions(p_alloc_func, p_free_func, p_user_data);
	}


}

