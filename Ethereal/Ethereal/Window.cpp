#include "Window.h"
#include "Events/ApplicationEvent.h"
#include "Events/KeyEvent.h"

namespace Ethereal
{
	Window::Window(const Properties& props)
	{
		Initialize(props);
	}

	Window::~Window()
	{
		glfwDestroyWindow(window);
	}

	std::unique_ptr<Window> Window::Create(const Properties& props)
	{
		return std::make_unique<Window>(props);
	}

	void Window::Initialize(const Properties& props)
	{
		data.width = props.width;
		data.height = props.height;
		data.title = props.title;

		if (!glfwInit())
			std::cout << "GLFW INIT ERROR";

		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
		glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
		glfwWindowHint(GLFW_OPENGL_COMPAT_PROFILE, GLFW_OPENGL_CORE_PROFILE);

		window = glfwCreateWindow(data.width, data.height, data.title.c_str(), nullptr, nullptr);

		if (!window)
			std::cout << "Window Creation error";

		SetCurrent(window);

		glfwSetWindowUserPointer(window, &data);

		glfwSetKeyCallback(window, [](GLFWwindow* window, int key, int scancode, int action, int mods)
			{
				Data& data = *(static_cast<Data*>(glfwGetWindowUserPointer(window)));
		
				KeyEvent event(key);
		
				data.callback(event);
			});

		glfwSetWindowCloseCallback(window, [](GLFWwindow* window)
			{
				Data& data = *(static_cast<Data*>(glfwGetWindowUserPointer(window)));
				CloseEvent event;
				data.callback(event);
			});
	}

	void Window::Update()
	{
		glfwPollEvents();
		glfwSwapBuffers(window);
	}

	void Window::SetVSync(bool enabled)
	{
		glfwSwapInterval(enabled);

		data.VSync = enabled;
	}

	void Window::SetCurrent(GLFWwindow* window)
	{
		glfwMakeContextCurrent(window);
	}

	void Window::SetEventCallback(const EventCallback& callback)
	{
		data.callback = callback;
	}

	Properties::Properties(int w, int h, const std::string& title)
		: width(w), height(h), title(title)
	{

	}
}