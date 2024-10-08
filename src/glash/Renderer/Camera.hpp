#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace Cine
{
	class Camera
	{
	public:
		Camera() = default;
		virtual ~Camera() = default;

		const glm::mat4& GetViewMatrix() const { return m_ViewMatrix; }
		const glm::mat4& GetProjectionMatrix() const { return m_ProjectionMatrix; }
		const glm::mat4& GetViewProjectionMatrix() const { return m_ViewProjectionMatrix; }

	protected:
		glm::mat4 m_ViewMatrix = glm::mat4(1.0f);
		glm::mat4 m_ProjectionMatrix = glm::mat4(1.0f);
		glm::mat4 m_ViewProjectionMatrix = glm::mat4(1.0f);
	};

	class OrthographicCamera : public Camera
	{
	public:
		OrthographicCamera(float left, float right, float bottom, float top);

		void SetProjection(float left, float right, float bottom, float top);

		void SetRotation(float rotation);
		void SetPosition(const glm::vec3& position);
		void SetTransform(const glm::vec3& position, float rotation);

		const float GetRotation() const { return m_Rotation; }
		const glm::vec3& GetPosition() const { return m_Position; }

	private:
		void RecalculateViewMatrix();

	private:
		glm::vec3 m_Position = { 0.0f, 0.0f, 0.0f };
		float m_Rotation = 0.0f; 
	};
}
