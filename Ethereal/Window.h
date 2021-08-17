#pragma once
#include <iostream>
#include <glfw3.h>

#include "EventSystem.h"

namespace Ethereal
{
	struct Properties
	{
		int width, height;
		std::string title;

		Properties(int w = 1280, int h = 720, const std::string& title = "OpenGL");
	};

	class Window final
	{
	public:

		using EventCallback = std::function<void(Event&)>;

		Window(const Properties& props);

		~Window();

		static std::unique_ptr<Window> Create(const Properties& props = Properties());

		void Update();

		static void SetCurrent(GLFWwindow* window);

		void SetEventCallback(const EventCallback& callback);

	private:

		GLFWwindow* window;

		//GraphicsContext::RenderContext* context;

		struct Data
		{
			int width, height;
			std::string title;

			EventCallback callback;
		} data;

		void Initialize(const Properties& props);
	};
}


