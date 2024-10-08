#include "glash/Renderer/Camera.hpp"
#include "glash/Core/Timestep.hpp"

#include "glash/Events/ApplicationEvent.hpp"
#include "glash/Events/MouseEvent.hpp"

namespace Cine
{
	class OrthograhpicCameraController
	{
	public:
		OrthograhpicCameraController(float aspectRation, bool canRotate = false);

		void OnEvent(Event& event);
		void OnUpdate(Timestep ts);

		const Camera& GetCamera() const { return m_Camera; }
		Camera& GetCamera() { return m_Camera; }

	private:
		bool OnMouseScrolled(MouseScrolledEvent& event);
		bool OnWindowResized(WindowResizeEvent& event);


	public:
		float CameraRotationSpeed = 45.0f;
		float CameraTranslatoinSpeed = 2.0f;
		float CameraZoomSpeed = 0.05f;

	private:
		bool m_CanRotate = false;
		float m_AspectRatio;

		glm::vec3 m_CameraPosition = glm::vec3(0.0f);
		float m_CameraZoom = 1.0f;
		float m_CameraRotation = 0.0f;

		float m_CameraZoomMax = 0.1f;

		OrthographicCamera m_Camera;
	};
}