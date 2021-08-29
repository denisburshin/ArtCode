#pragma once

#include <Ethereal/Events/KeyEvent.h>

namespace Ethereal
{
	class IApplication
	{
	public:
		IApplication() = default;
		virtual ~IApplication() = default;

		virtual void OnUpdate() {};
		virtual void OnEvent(Event& event) = 0;

		virtual bool OnKeyEvent(KeyEvent& event) = 0;

		virtual void OnGUIRender() {};
	};
}