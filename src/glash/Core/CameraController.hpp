#pragma once

#include "glash/Renderer/OrthographicCamera.hpp"
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

		void OnResize(float width, float heigh);

		const OrthographicCamera& GetCamera() const { return m_Camera; }
		OrthographicCamera& GetCamera() { return m_Camera; }

		inline void SetAspectRatio(float aspectRatio) { m_AspectRatio = aspectRatio; }
		void SetTargetZoom(float targetZoom);


		void Reset();

	private:
		bool OnMouseScrolled(MouseScrolledEvent& event);
		bool OnWindowResized(WindowResizeEvent& event);


	public:
		float CameraRotationSpeed = 90.0f;
		float CameraTranslationSpeed = 2.0f;
		float CameraZoomSpeed = 0.1f;

	private:
		bool m_CanRotate = false;
		float m_AspectRatio = 1.0f;

		glm::vec3 m_CameraPosition = glm::vec3(0.0f);
		float m_CameraRotation = 0.0f;
		float m_CameraTranslatioSpeedBoost = 10.0f;

		float m_CameraZoom = 1.0f;
		float m_CameraZoomMin = 0.1f;
		float m_CameraZoomTarget = m_CameraZoom;
		float m_CameraZoomTargetTime = 0.1f;
		float m_CameraZoomSpeedBoost = 10.0f;

		OrthographicCamera m_Camera;
	};
}