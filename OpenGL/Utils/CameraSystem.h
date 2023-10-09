#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Camera.h"
#include "../Core/Event.h"


namespace GL
{
	class CameraSystem
	{
	public:
		CameraSystem(uint16_t width, uint16_t height);
		~CameraSystem();

		void OnUpdate(float deltaTime);
		void OnEvent(Event& event);

		void SetOrthographicProjection();
		void SetPerspectiveProjection();

		Camera* GetCamera() { return m_Camera; };
		const Camera* GetCamera() const { return m_Camera; };

		const glm::mat4& GetViewMatrix() const { return m_ViewMatrix; }

		void SetCameraPosition(const glm::vec3& position) { m_CameraPosition = position; CalculateViewMatrix(); };

		void CalculateViewMatrix();

	private:
		bool OnMouseScrolled(const MouseScrolledEvent& event);
		bool OnMouseMoved(const MouseMovedEvent& event);
		bool OnResize(const WindowResizeEvent& event);

	private:
		float m_ViewportWidth = 1280, m_ViewportHeight = 720;
		float m_Pitch = 0.0f, m_Yaw = 0.0f;
		float m_CameraRotation = 0.0f; // In-degree
		float m_MouseSensitivity = 0.002f, m_TranslationSpeed = 2.5f, m_RotationSpeed = 180.0f;

		glm::vec2 m_InitialMousePosition = { 0.0f, 0.0f };

		glm::vec3 m_CameraPosition = glm::vec3(0.0f, 0.0f, 0.0f); // For 3d scene
		glm::vec3 m_CameraFrontDirection = glm::vec3(0.0f, 0.0f, -1.0f);
		glm::vec3 m_CameraUpDirection = glm::vec3(0.0f, 1.0f, 0.0f);
		glm::vec3 m_CameraRightDirection = glm::vec3(1.0f, 0.0f, 0.0f);

		glm::mat4 m_ViewMatrix = glm::mat4(1.0f);

		Camera* m_Camera = nullptr;
	};
};

