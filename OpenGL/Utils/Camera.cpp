#include "Camera.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


namespace GL {

	Camera::Camera(float aspectRatio): m_AspectRatio(aspectRatio)
	{
		UpdateProjection();
	};


	void Camera::UpdateProjection()
	{
		if (m_ProjectionType == ProjectionType::PERSPECTIVE)
		{
			m_ProjectionMatrix = glm::perspective(glm::radians(m_PerspectiveFov), m_AspectRatio, m_PerspectiveNearClip, m_PerspectiveFarClip);
		}
		else
		{
			float left = -m_OrthographicSize * m_AspectRatio;
			float right = m_OrthographicSize * m_AspectRatio;
			float bottom = -m_OrthographicSize * 1.0f;
			float top = m_OrthographicSize * 1.0f;
			m_ProjectionMatrix = glm::ortho(left, right, bottom, top, m_OrthographicNearClip, m_OrthographicFarClip);
		}
	};


	void Camera::SetOrthographic(float size, float nearClip, float farClip)
	{
		m_ProjectionType = ProjectionType::ORTHOGRAPHIC;
		m_OrthographicSize = size;
		m_OrthographicNearClip = nearClip;
		m_OrthographicFarClip = farClip;
		UpdateProjection();
	};


	void Camera::SetPerspective(float fov, float nearClip, float farClip)
	{
		m_ProjectionType = ProjectionType::PERSPECTIVE;
		m_PerspectiveFov = fov;
		m_PerspectiveNearClip = nearClip;
		m_PerspectiveFarClip = farClip;
		UpdateProjection();
	};


	void Camera::SetViewportSize(uint32_t width, uint32_t height)
	{
		m_AspectRatio = (float)width / (float)height;
		UpdateProjection();
	}

}