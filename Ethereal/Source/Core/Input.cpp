#include "Input.h"
#include <Core/Application.h>
#include <GLFW/glfw3.h>

namespace Ethereal
{
	Input* Input::instance = new Input;

	bool Input::IsKeyPressed(Key key)
	{
		auto window = static_cast<GLFWwindow*>(Application::Get()->GetWindow().GetNativeWindow());

		auto state = glfwGetKey(window, static_cast<int>(key));

		return state == GLFW_PRESS || state == GLFW_REPEAT;
	}

	bool Input::IsMouseButtonPressed(Mouse button)
	{
		auto window = static_cast<GLFWwindow*>(Application::Get()->GetWindow().GetNativeWindow());

		auto state = glfwGetMouseButton(window, static_cast<int>(button));

		return state == GLFW_PRESS || state == GLFW_REPEAT;
	}

	glm::vec2 Input::GetMousePosition()
	{
		auto window = static_cast<GLFWwindow*>(Application::Get()->GetWindow().GetNativeWindow());
		double xPos, yPos;
		glfwGetCursorPos(window, &xPos, &yPos);
		return glm::vec2((float)xPos, (float)yPos);
	}

	float Input::GetMouseX()
	{
		return GetMousePosition().x;
	}

	float Input::GetMouseY()
	{
		return GetMousePosition().y;
	}
}
