#pragma once
#include <Core/Window.h>
#include <Core/Events/ApplicationEvent.h>
#include <Core/Layer.h>
#include <Core/GUI/GUI.h>
#include <Core/Render/Renderer.h>
#include <memory>

int main();

namespace Ethereal
{
	class Application
	{
	public:
		Application();
		~Application() = default;

		void OnUpdate();

		void Close();

		void OnEvent(Event& e);

		void PushLayer(Layer* layer);

		Window& GetWindow();

		static Application* Get();
		using LayerStack = std::vector<std::unique_ptr<Layer>>;
	private:
		void Run();
		bool Resize(ResizeEvent& e);
		bool WindowClose(CloseEvent& e);
	private:
		bool running;
		std::unique_ptr<GUI> gui;
		std::unique_ptr<Window> window;
	private:
		LayerStack layers;
	private:
		static Application* instance;
	private:
		friend int ::main();
	};

	static Application* CreateApplication();
};

