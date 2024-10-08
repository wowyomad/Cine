#include "glash/Cine.hpp"

class Sandbox2D : public Cine::Layer
{
public:
	Sandbox2D() : Cine::Layer("Sandbox"),
		m_CameraController(1.7778)
	{
			
	};

	void OnAttach() override final;
	void OnDetach() override final;

	void OnUpdate(Cine::Timestep ts) override final;
	void OnEvent(Cine::Event& event) override final;
	void OnImGuiRender() override final;


private:
	Cine::OrthograhpicCameraController m_CameraController;

	Cine::Ref<Cine::VertexArray> m_SquareVertexArray;
	Cine::Ref<Cine::Shader> m_SquareShader;

	glm::vec4 m_SquareColor = glm::vec4(0.4f, 0.2f, 0.7f, 1.0f);

};