#include <Platform/WindowsWindow.h>
#include <Ethereal/Events/ApplicationEvent.h>
#include <Ethereal/Events/KeyEvent.h>

namespace Ethereal
{
	WindowsWindow::WindowsWindow(const Properties& props)
	{
		Initialize(props);
	}

	WindowsWindow::~WindowsWindow()
	{
		glfwDestroyWindow(window);
	}

	uint32_t WindowsWindow::GetWidth() const
	{
		return data.width;
	}

	uint32_t WindowsWindow::GetHeight() const
	{
		return data.height;
	}

	void WindowsWindow::Initialize(const Properties& props)
	{
		data.width = props.width;
		data.height = props.height;
		data.title = props.title;

		if (!glfwInit())
		{
			std::cout << "GLFW INIT ERROR";
			glfwTerminate();
		}
			

		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
		glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
		glfwWindowHint(GLFW_OPENGL_COMPAT_PROFILE, GLFW_OPENGL_CORE_PROFILE);

		window = glfwCreateWindow(data.width, data.height, data.title.c_str(), nullptr, nullptr);

		if (!window)
			std::cout << "WindowsWindow Creation error";

		glfwMakeContextCurrent(window);

		glfwSetWindowUserPointer(window, &data);

		glfwSetKeyCallback(window, [](GLFWwindow* WindowsWindow, int key, int scancode, int action, int mods)
			{
				Data& data = *(static_cast<Data*>(glfwGetWindowUserPointer(WindowsWindow)));
		
				KeyEvent event(key);
		
				data.callback(event);
			});

		glfwSetWindowCloseCallback(window, [](GLFWwindow* WindowsWindow)
			{
				Data& data = *(static_cast<Data*>(glfwGetWindowUserPointer(WindowsWindow)));
				CloseEvent event;
				data.callback(event);
			});
	}

	void WindowsWindow::OnUpdate()
	{
		glfwPollEvents();
		glfwSwapBuffers(window);
	}

	void WindowsWindow::SetVSync(bool enabled)
	{
		glfwSwapInterval(enabled);

		data.VSync = enabled;
	}

	bool WindowsWindow::IsVSync() const
	{
		return data.VSync;
	}

	void* WindowsWindow::GetNativeWindow() const
	{
		return static_cast<GLFWwindow*>(window);
	}

	void WindowsWindow::SetEventCallback(const EventCallback& callback)
	{
		data.callback = callback;
	}

	Properties::Properties(int w, int h, const std::string& title)
		: width(w), height(h), title(title)
	{

	}
}