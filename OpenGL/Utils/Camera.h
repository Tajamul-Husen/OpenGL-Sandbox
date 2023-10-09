#pragma once

#include <glm/glm.hpp>


namespace GL {

	enum ProjectionType {
		ORTHOGRAPHIC,
		PERSPECTIVE
	};


	class Camera
	{
	public:
		Camera();
		Camera(float aspectRatio);
		~Camera() = default;

		const glm::mat4& GetProjectionMatrix() const { return m_ProjectionMatrix; };

		ProjectionType GetProjectionType() const { return m_ProjectionType; }
		void SetProjectionType(ProjectionType type) { m_ProjectionType = type; UpdateProjection(); };

		float GetPerspectiveFov() { return m_PerspectiveFov; };
		void SetPerspectiveFov(float fov) { m_PerspectiveFov = fov; UpdateProjection(); };

		float GetOrthographicSize() { return m_OrthographicSize; };
		void SetOrthographicSize(float size) { m_OrthographicSize = size; UpdateProjection(); };

		void SetOrthographic(float size, float nearClip, float farClip);
		void SetPerspective(float fov, float nearClip, float farClip);

		void SetViewportSize(uint32_t width, uint32_t height);

	private:
		void UpdateProjection();

	private:
		float m_AspectRatio = 1.778f;

		float m_OrthographicSize = 1.0f;
		float m_OrthographicNearClip = -1.0f, m_OrthographicFarClip = 1.0f;

		float m_PerspectiveFov = 45.0f;
		float m_PerspectiveNearClip = 0.1f, m_PerspectiveFarClip = 1000.0f;

		ProjectionType m_ProjectionType = ProjectionType::ORTHOGRAPHIC;

		glm::mat4 m_ProjectionMatrix = glm::mat4(1.0f);
	};

}
