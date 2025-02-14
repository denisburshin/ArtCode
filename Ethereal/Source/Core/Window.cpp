#include <Core/Window.h>
#include <Platform/Windows/WindowsWindow.h>

namespace Ethereal
{
	std::unique_ptr<Window> Window::Create(const Properties& props)
	{
		return std::make_unique<WindowsWindow>(props);
	}
};