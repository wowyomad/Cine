#include "glash/Cine.hpp"


struct ProfileResult
{
	const char* Name;
	float Time;
};

class Sandbox2D : public Cine::Layer
{
public:
	Sandbox2D(Cine::Application* ptr) : Cine::Layer("Sandbox"),
		m_CameraController(1.7778)
	{
		s_Application = ptr;
	};

	void OnAttach() override final;
	void OnDetach() override final;

	void OnUpdate(Cine::Timestep ts) override final;
	void OnEvent(Cine::Event& event) override final;
	void OnImGuiRender() override final;


private:
	Cine::OrthograhpicCameraController m_CameraController;

	float m_SquareRotation = 0.0f;
	glm::vec4 m_SquareColor = glm::vec4(0.4f, 0.2f, 0.7f, 1.0f);
	glm::vec2 m_SquareSize = glm::vec2(1.0f);
	glm::vec3 m_SquarePosition = glm::vec3(0.0f);
	Cine::Ref<Cine::Texture2D> m_CheckerBoardTexture;
	Cine::Ref<Cine::Texture2D> m_FaceTexture;


	bool m_VSync = false;
	Cine::Application* s_Application = nullptr;

	std::vector<ProfileResult> m_ProfileResults;
};