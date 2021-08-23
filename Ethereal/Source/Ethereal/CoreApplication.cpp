#include <glad/gl.h>
#include <GLFW/glfw3.h>

#include <Ethereal/CoreApplication.h>
#include <Utility/Time.h>

namespace Ethereal
{
	CoreApplication* CoreApplication::appInstance = nullptr;

	CoreApplication::CoreApplication()
	{
		window = Window::Create();

		window->SetEventCallback(std::bind(&CoreApplication::OnEvent, this, std::placeholders::_1));

		window->SetVSync(true);

		auto version = gladLoadGL(glfwGetProcAddress);
		if (!version)
			std::cout << "GLAD INIT ERROR" << std::endl;
		
		appInstance = this;
	}

	CoreApplication::~CoreApplication()
	{
		delete application;
	}

	void CoreApplication::OnEvent(Event& e)
	{
		std::cout << Time::GetTime() << ": " << e.GetName() << std::endl;

		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<CloseEvent>(std::bind(&CoreApplication::Close, this, std::placeholders::_1));

		application->OnEvent(e);
	}

	void CoreApplication::OnUpdate()
	{
		static bool flag = false;
		if (!flag)
		{
			std::cout << "Update function not set" << std::endl;
			flag = true;
		}
	}

	void CoreApplication::PushApplication(IApplication* app)
	{
		application = app;
	}

	bool CoreApplication::Close(CloseEvent& e)
	{
		running = false;
		return running;
	}

	void CoreApplication::Run()
	{
		running = true;
		while (running)
		{
			application->OnUpdate();
			window->OnUpdate();
		}
	}
};