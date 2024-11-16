#pragma once

#include "glash/Renderer/Camera.hpp"

namespace Cine
{
	class SceneCamera : public Camera
	{
	public:
		enum class ProjectionType { Perspective = 0, Orthographic = 1 };
		struct PerspectiveData
		{
			float FOV = glm::radians(45.0f);
			float Near = 0.01f, Far = 1000.0f;
		
		};
		struct OrthographicData
		{
			float Size = 10.0f;
			float Near = -1.0f, Far = 1.0f;
		};
	
	public:
		SceneCamera();
		virtual ~SceneCamera() = default;

		void SetOrthographic(float size, float nearClip, float farClip);
		void SetOrthographicSize(float size) { m_OrthographicData.Size = size; RecalculateProjection(); }
		void SetOrthographicNearClip(float nearClip) { m_OrthographicData.Near = nearClip; RecalculateProjection(); }
		void SetOrthographicFarClip(float farClip) { m_OrthographicData.Far = farClip; RecalculateProjection(); }

		void SetPerspective(float fov, float nearClip, float farClip);
		void SetPerspectiveVerticalFOV(float fov) {m_PerspectiveData.FOV = fov; RecalculateProjection(); }
		void SetPerspectiveNearClip(float nearClip) { m_PerspectiveData.Near = nearClip; RecalculateProjection(); }
		void SetPerspectiveFarClip(float farClip) { m_PerspectiveData.Far = farClip; RecalculateProjection(); }

		float GetOrthographicSize() const { return m_OrthographicData.Size; }
		float GetOrthographicNearClip() const { return m_OrthographicData.Near; }
		float GetOrthographicFarClip () const { return m_OrthographicData.Far; }

		float GetPerspectiveVerticalFOV() const { return m_PerspectiveData.FOV; }
		float GetPerspectiveNearClip() const { return m_PerspectiveData.Near; }
		float GetPerspectiveFarClip() const { return m_PerspectiveData.Far; }

		glm::mat4 GetViewMatrix() const;

		bool IsOrthographic() const { return m_ProjectionType == ProjectionType::Orthographic; }
		bool IsPerspective() const { return m_ProjectionType == ProjectionType::Perspective; }


		ProjectionType GetProjectionType() const { return m_ProjectionType; }
		void SetProjectionType(ProjectionType type);

		void SetViewportSize(uint32_t width, uint32_t height);

	private:
		ProjectionType m_ProjectionType = ProjectionType::Orthographic;

		PerspectiveData m_PerspectiveData;
		OrthographicData m_OrthographicData;

		float m_AspectRatio = 0.0f;

	private:
		void RecalculateProjection();
	};
}