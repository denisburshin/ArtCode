#include <glad/gl.h>
#include <GLFW/glfw3.h>

#include <Core/Application.h>
#include <Utility/Time.h>

namespace Ethereal
{
	Application* Application::instance = nullptr;

	Application::Application()
	{
		window = Window::Create();

		window->SetEventCallback(std::bind(&Application::OnEvent, this, std::placeholders::_1));

		window->SetVSync(true);

		auto version = gladLoadGL(glfwGetProcAddress);
		if (!version)
			std::cout << "GLAD INIT ERROR" << std::endl;
		
		instance = this;

		gui = std::make_unique<GUI>();
	}

	void Application::OnEvent(Event& e)
	{
		std::cout << Time::GetTime() << ": " << e.GetName() << std::endl;

		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<CloseEvent>(std::bind(&Application::WindowClose, this, std::placeholders::_1));
		dispatcher.Dispatch<ResizeEvent>(std::bind(&Application::Resize, this, std::placeholders::_1));
		
		for (auto &layer : layers)
		{
			layer->OnEvent(e);
			if (e.handled)
				break;
		}

	}

	void Application::OnUpdate()
	{
		static bool flag = false;
		if (!flag)
		{
			std::cout << "Update function not set" << std::endl;
			flag = true;
		}
	}

	void Application::PushLayer(Layer* layer)
	{
		layers.push_back(layer->GetLayer());
	}

	Application* Application::Get()
	{
		return instance;
	}

	Window& Application::GetWindow()
	{
		return *window;
	}

	bool Application::WindowClose(CloseEvent& e)
	{
		running = false;
		return running;
	}

	void Application::Close()
	{
		running = false;
	}

	// TODO: Resize Models
	bool Application::Resize(ResizeEvent& e)
	{
		Renderer::SetViewport(e.GetWidth(), e.GetHeight());
		return false;
	}

	void Application::Run()
	{
		running = true;
		while (running)
		{
			for (auto& layer : layers)
				layer->OnUpdate();

			gui->Begin();
			{
				for (auto& layer : layers)
					layer->OnGUIRender();
			}
			gui->End();

			window->OnUpdate();
		}
	}
};