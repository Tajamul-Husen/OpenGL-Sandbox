#include "Input.h"
#include "Application.h"


namespace GL {

	bool Input::IsKeyPressed(int key)
	{
		auto window = static_cast<GLFWwindow*>(Application::Get()->GetWindow());

		auto status = glfwGetKey(window, key);
		return status == GLFW_PRESS;
	};


	bool Input::IsMouseButtonPressed(int button)
	{
		auto window = static_cast<GLFWwindow*>(Application::Get()->GetWindow());

		auto state = glfwGetMouseButton(window, button);
		return state == GLFW_PRESS;
	};


	std::pair<float, float> Input::GetMousePosition()
	{
		auto window = static_cast<GLFWwindow*>(Application::Get()->GetWindow());

		double xpos, ypos;
		glfwGetCursorPos(window, &xpos, &ypos);

		return { (float)xpos, (float)ypos };
	}


	float Input::GetMouseX()
	{
		auto [x, y] = Input::GetMousePosition();
		return x;
	}


	float Input::GetMouseY()
	{
		auto [x, y] = Input::GetMousePosition();
		return y;
	}

};