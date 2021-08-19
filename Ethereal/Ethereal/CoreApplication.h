#pragma once
#include "Window.h"
#include "Events/ApplicationEvent.h"
#include "IApplication.h"
#include "Timestep.h"

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
		Timestep timestep;
		float lastTime;
		static CoreApplication* appInstance;
	private:
		friend int ::main();
	};

	static CoreApplication* CreateApplication();
};

