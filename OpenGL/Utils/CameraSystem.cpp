#include "CameraSystem.h"
#include "../Core/Input.h"
#include "../Debug/Log.h"


namespace GL
{

	CameraSystem::CameraSystem(uint16_t width, uint16_t height) : m_ViewportWidth(width), m_ViewportHeight(height)
	{
		m_Camera = new Camera(static_cast<float>(width / height));
	};


	CameraSystem::~CameraSystem()
	{
		delete m_Camera;
	};


	void CameraSystem::OnUpdate(float deltaTime)
	{
		float velocity = m_TranslationSpeed * deltaTime;

		if (Input::IsKeyPressed(Key::W))
		{
			if (m_Camera->GetProjectionType() == ProjectionType::ORTHOGRAPHIC)
			{
				m_CameraPosition.y += velocity;
			}
			else {
				m_CameraPosition += (m_CameraFrontDirection * velocity);
			}
		}

		if (Input::IsKeyPressed(Key::S))
		{
			if (m_Camera->GetProjectionType() == ProjectionType::ORTHOGRAPHIC)
			{
				m_CameraPosition.y -= velocity;
			}
			else
			{
				m_CameraPosition -= (m_CameraFrontDirection * velocity);
			}
		}

		if (Input::IsKeyPressed(Key::A))
		{
			if (m_Camera->GetProjectionType() == ProjectionType::ORTHOGRAPHIC)
			{
				m_CameraPosition.x -= velocity;
			}
			else {
				m_CameraPosition -= (m_CameraRightDirection * velocity);
			}
		}

		if (Input::IsKeyPressed(Key::D))
		{
			if (m_Camera->GetProjectionType() == ProjectionType::ORTHOGRAPHIC)
			{
				m_CameraPosition.x += velocity;
			}
			else
			{
				m_CameraPosition += (m_CameraRightDirection * velocity);
			}
		}

		if (m_Camera->GetProjectionType() == ProjectionType::ORTHOGRAPHIC)
		{
			if (Input::IsKeyPressed(Key::Q))
				m_CameraRotation += m_RotationSpeed * deltaTime;
			if (Input::IsKeyPressed(Key::E))
				m_CameraRotation -= m_RotationSpeed * deltaTime;

			if (m_CameraRotation > 180.0f)
				m_CameraRotation -= 360.0f;
			else if (m_CameraRotation <= -180.0f)
				m_CameraRotation += 360.0f;

		}

		CalculateViewMatrix();
	};


	void CameraSystem::CalculateViewMatrix()
	{

		if (m_Camera->GetProjectionType() == ProjectionType::PERSPECTIVE)
		{
			glm::quat qPitch = glm::angleAxis(m_Pitch, glm::vec3(1, 0, 0));
			glm::quat qYaw = glm::angleAxis(m_Yaw, glm::vec3(0, 1, 0));
			glm::quat orientation = glm::normalize(qPitch * qYaw);

			glm::mat4 rotate = glm::mat4_cast(orientation);
			glm::mat4 translate = glm::translate(glm::mat4(1.0f), m_CameraPosition);

			m_ViewMatrix = glm::inverse(translate * rotate);
		}
		else {
			glm::mat4 translate = glm::translate(glm::mat4(1.0f), m_CameraPosition);
			glm::mat4 rotate = glm::rotate(glm::mat4(1.0f), glm::radians(m_CameraRotation), glm::vec3(0, 0, 1));

			m_ViewMatrix = glm::inverse(translate * rotate);
		}
	};


	void CameraSystem::OnEvent(Event& event)
	{
		EventDispatcher dispatcher(event);

		dispatcher.Dispatch<WindowResizeEvent>(BIND_EVENT_FN(CameraSystem::OnResize));
		dispatcher.Dispatch<MouseMovedEvent>(BIND_EVENT_FN(CameraSystem::OnMouseMoved));
		dispatcher.Dispatch<MouseScrolledEvent>(BIND_EVENT_FN(CameraSystem::OnMouseScrolled));
	};


	bool CameraSystem::OnMouseScrolled(const MouseScrolledEvent& event)
	{
		float yOffset = event.GetYOffset();

		if (m_Camera->GetProjectionType() == ProjectionType::PERSPECTIVE)
		{
			float fov = m_Camera->GetPerspectiveFov();

			fov -= yOffset * 0.8f;

			if (fov < 1.0f) {
				fov = 1.0f;
			}

			if (fov > 90.0f) {
				fov = 90.0f;
			}
			m_Camera->SetPerspectiveFov(fov);
		}
		else {
			float zoomLevel = m_Camera->GetOrthographicSize();

			zoomLevel -= yOffset * 0.8f;
			zoomLevel = std::max(zoomLevel, 0.8f);

			m_Camera->SetOrthographicSize(zoomLevel);
		}

		return false;
	};


	bool CameraSystem::OnMouseMoved(const MouseMovedEvent& event)
	{
		glm::vec2 mouse = { event.GetXPos(), event.GetYPos() };
		glm::vec2 delta;

		delta.x = (mouse.x - m_InitialMousePosition.x);
		delta.y = (m_InitialMousePosition.y - mouse.y);

		m_InitialMousePosition = mouse;

		if (m_Camera->GetProjectionType() == ProjectionType::PERSPECTIVE) {

			if (Input::IsKeyPressed(Key::LeftControl) && Input::IsMouseButtonPressed(Mouse::ButtonLeft))
			{
				m_Yaw += delta.x * m_MouseSensitivity;
				m_Pitch += delta.y * m_MouseSensitivity;

				// make sure that when pitch is out of bounds, screen doesn't get flipped
				if (m_Pitch > 89.0f)
					m_Pitch = 89.0f;
				if (m_Pitch < -89.0f)
					m_Pitch = -89.0f;
			}

			CalculateViewMatrix();
		}

		return false;
	}


	bool CameraSystem::OnResize(const WindowResizeEvent& event)
	{
		m_Camera->SetViewportSize((float)event.GetWidth(), (float)event.GetHeight());
		return false;
	}


	void CameraSystem::SetOrthographicProjection()
	{
		m_Camera->SetProjectionType(ProjectionType::ORTHOGRAPHIC);
	};


	void CameraSystem::SetPerspectiveProjection()
	{
		m_Camera->SetProjectionType(ProjectionType::PERSPECTIVE);
	};

};