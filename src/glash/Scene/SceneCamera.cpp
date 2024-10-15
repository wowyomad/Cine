#include "glash/glash_pch.hpp"
#include "SceneCamera.hpp"

namespace Cine
{
	SceneCamera::SceneCamera()
	{

	}

	void SceneCamera::SetOrthographic(float size, float nearClip, float farClip)
	{
		m_OrthographicData.Size = size;
		m_OrthographicData.Near = nearClip;
		m_OrthographicData.Far = farClip;
		RecalculateProjection();
	}

	void SceneCamera::SetPerspective(float fov, float nearClip, float farClip)
	{
		m_PerspectiveData.FOV = fov;
		m_PerspectiveData.Near = nearClip;
		m_PerspectiveData.Far = farClip;
		RecalculateProjection();
	}

	void SceneCamera::SetProjectionType(ProjectionType type)
	{
		m_ProjectionType = type;
		RecalculateProjection();
	}

	void SceneCamera::SetViewportSize(uint32_t width, uint32_t height)
	{
		m_AspectRatio = static_cast<float>(width) / static_cast<float>(height);
		RecalculateProjection();
	}

	void SceneCamera::RecalculateProjection()
	{

		switch (m_ProjectionType)
		{
		case ProjectionType::Orthographic:
		{
			float size = m_OrthographicData.Size;
			float orthoLeft = -size * m_AspectRatio * 0.5f;
			float orthoRight = size * m_AspectRatio * 0.5f;
			float orthoTop = size * 0.5f;
			float orthoBottom = -size * 0.5f;

			m_Projection = glm::ortho(orthoLeft, orthoRight, orthoBottom, orthoTop, m_OrthographicData.Near, m_OrthographicData.Far);
		} break;
		case ProjectionType::Perspective:
		{
			m_Projection = glm::perspective(m_PerspectiveData.FOV, m_AspectRatio, m_PerspectiveData.Near, m_PerspectiveData.Far);
		} break;
		}

	}
}

