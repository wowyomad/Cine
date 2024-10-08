#include "glash/glash_pch.hpp"
#include "glash/Core/CameraController.hpp"

namespace Cine
{
	OrthograhpicCameraController::OrthograhpicCameraController(float aspectRatio, bool canRotate)
		: m_AspectRatio(aspectRatio),
		m_CanRotate(canRotate),
		m_CameraZoom(1.0f),
		m_Camera(-m_AspectRatio * m_CameraZoom, m_AspectRatio * m_CameraZoom, -m_CameraZoom, m_CameraZoom)
	{

	}

	void OrthograhpicCameraController::OnEvent(Event& event)
	{
		EventDispatcher dispatcher(event);
		dispatcher.Dispatch<MouseScrolledEvent>(CINE_BIND_EVENT_FN(OrthograhpicCameraController::OnMouseScrolled));
		dispatcher.Dispatch<WindowResizeEvent>(CINE_BIND_EVENT_FN(OrthograhpicCameraController::OnWindowResized));
	}

	void OrthograhpicCameraController::OnUpdate(Timestep ts)
	{
		if (Input::IsKeyPressed(Key::A))
		{
			m_CameraPosition.x -= CameraTranslatoinSpeed * m_CameraZoom * ts;
		}
		if (Input::IsKeyPressed(Key::D))
		{
			m_CameraPosition.x += CameraTranslatoinSpeed * m_CameraZoom * ts;
		}
		if (Input::IsKeyPressed(Key::W))
		{
			m_CameraPosition.y += CameraTranslatoinSpeed * m_CameraZoom * ts;
		}
		if (Input::IsKeyPressed(Key::S))
		{
			m_CameraPosition.y -= CameraTranslatoinSpeed * m_CameraZoom * ts;
		}

		if (m_CanRotate)
		{
			if (Input::IsKeyPressed(Key::E))
			{
				m_CameraRotation -= CameraRotationSpeed * ts;
			}
			if (Input::IsKeyPressed(Key::Q))
			{
				m_CameraRotation += CameraRotationSpeed * ts;
			}
		}

		m_Camera.SetTransform(m_CameraPosition, m_CameraRotation);
	}

	bool OrthograhpicCameraController::OnMouseScrolled(MouseScrolledEvent& event)
	{
		m_CameraZoom -= event.GetVertical() * CameraZoomSpeed;
		m_CameraZoom = std::max(m_CameraZoom, m_CameraZoomMax);
		m_Camera.SetProjection(-m_AspectRatio * m_CameraZoom, m_AspectRatio * m_CameraZoom, -m_CameraZoom, m_CameraZoom);

		return false;
	}

	bool OrthograhpicCameraController::OnWindowResized(WindowResizeEvent& event)
	{
		m_AspectRatio = (float)event.GetWidth() / (float)event.GetHeight();
		m_Camera.SetProjection(-m_AspectRatio * m_CameraZoom, m_AspectRatio * m_CameraZoom, -m_CameraZoom, m_CameraZoom);

		return false;
	}
}