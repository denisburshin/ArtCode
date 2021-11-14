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
}
