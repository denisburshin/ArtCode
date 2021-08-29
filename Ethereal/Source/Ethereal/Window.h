#pragma once
#include <Ethereal/Events/EventSystem.h>

namespace Ethereal
{
	struct Properties
	{
		int width, height;
		std::string title;

		Properties(int w = 1280, int h = 720, const std::string& title = "ArtCode");
	};

	class Window
	{
	public:
		virtual ~Window() = default;

		using EventCallback = std::function<void(Event&)>;

		virtual void OnUpdate() = 0;

		virtual uint32_t GetWidth() const = 0;
		virtual uint32_t GetHeight() const = 0;

		// Window attributes
		virtual void SetEventCallback(const EventCallback& callback) = 0;
		virtual void SetVSync(bool enabled) = 0;
		virtual bool IsVSync() const = 0;

		virtual void* GetNativeWindow() const = 0;

		static std::unique_ptr<Window> Create(const Properties& props = Properties());
	};
}



