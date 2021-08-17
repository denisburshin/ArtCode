#pragma once
#include "Window.h"
#include "ApplicationEvent.h"

#include <memory>

int main();

namespace Ethereal
{
	
	class CoreApplication
	{
	public:
		CoreApplication();
		virtual ~CoreApplication() = default;

		void OnEvent(Event& e);

		virtual void OnUpdate() = 0;

	private:
		void Run();
		bool Close(CloseEvent& e);
	private:
		bool running;
		std::unique_ptr<Window> window;
		static CoreApplication* appInstance;
	private:
		friend int ::main();
	};

	static CoreApplication* CreateApplication();
};

