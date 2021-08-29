#pragma once

#include <Ethereal/Window.h>
#include <glfw/glfw3.h>

namespace Ethereal
{
	class WindowsWindow final : public Window
	{
	public:
		WindowsWindow(const Properties& props);
		virtual ~WindowsWindow();

		uint32_t GetWidth() const override;
		uint32_t GetHeight() const override;

		void OnUpdate() override;

		void SetVSync(bool enabled = false) override;
		bool IsVSync() const override;

		void* GetNativeWindow() const override;

		void SetEventCallback(const EventCallback& callback) override;

	private:

		GLFWwindow* window;

		//GraphicsContext::RenderContext* context;

		struct Data
		{
			int width, height;
			std::string title;

			bool VSync;

			EventCallback callback;
		} data;

		void Initialize(const Properties& props);
	};
}


