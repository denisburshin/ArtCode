#include <glad/gl.h>
#include "CoreApplication.h"

namespace Ethereal
{
	CoreApplication* CoreApplication::appInstance = nullptr;

	CoreApplication::CoreApplication()
	{
		window = Window::Create();

		window->SetEventCallback(std::bind(&CoreApplication::OnEvent, this, std::placeholders::_1));

		auto version = gladLoadGL(glfwGetProcAddress);
		if (!version)
			std::cout << "GLAD INIT ERROR" << std::endl;
		
		appInstance = this;
	}

	void CoreApplication::OnEvent(Event& e)
	{
		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<CloseEvent>(std::bind(&CoreApplication::Close, this, std::placeholders::_1));
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
			OnUpdate();
			window->Update();
		}
	}
};