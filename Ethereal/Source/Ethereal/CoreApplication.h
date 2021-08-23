#pragma once
#include <Ethereal/Window.h>
#include <Ethereal/Events/ApplicationEvent.h>
#include <Ethereal/IApplication.h>

#include <memory>

int main();

namespace Ethereal
{
	class CoreApplication
	{
	public:
		CoreApplication();
		~CoreApplication();

		void OnEvent(Event& e);

		void OnUpdate();

		void PushApplication(IApplication* app);

	private:
		void Run();
		bool Close(CloseEvent& e);
	private:
		bool running;
		std::unique_ptr<Window> window;
		IApplication* application;
	private:
		static CoreApplication* appInstance;
	private:
		friend int ::main();
	};

	static CoreApplication* CreateApplication();
};

