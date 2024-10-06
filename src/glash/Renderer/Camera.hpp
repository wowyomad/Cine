#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace Cine
{
	class Camera
	{
	public:
		Camera() = default;
		Camera(const glm::mat4& projection)
			: m_ProjectionMatrix(projection) {}

		virtual ~Camera() = default;

		const glm::mat4& GetViewProjection() const { return m_ViewProjectionMatrix; }
		const glm::mat4& GetProjection() const { return m_ProjectionMatrix; }

		
	protected:
		glm::mat4 m_ProjectionMatrix = glm::mat4(1.0f);
		glm::mat4 m_ViewProjectionMatrix = glm::mat4(1.0f);
		glm::vec3 m_Position = { 0.0f, 0.0f, 0.0f };

	};

	class OrthographicCamera : public Camera
	{
	public:
		OrthographicCamera(float left, float right, float bottom, float top);

		void SetProjection(float left, float right, float bottom, float top);
		const glm::mat4& GetViewMatrix() const { return m_ViewMatrix; }

		void SetRotation(float rotation) { m_Rotation = rotation; RecalculateViewMatrix(); }
		const float GetRotation() const { return m_Rotation; }

		void SetPosition(const glm::vec3& position) { m_Position = position; RecalculateViewMatrix(); }
		const glm::vec3& GetPosition() const { return m_Position; }


	protected:
		void RecalculateViewMatrix();

	private:
		glm::mat4 m_ViewMatrix = glm::mat4(1.0f);
		float m_Rotation = 0.0f; 
	};
}
