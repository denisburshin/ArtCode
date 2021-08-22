#pragma once

#include <Ethereal/Events/KeyEvent.h>
#include <Ethereal/Timestep.h>

namespace Ethereal
{
	class IApplication
	{
	public:
		IApplication() = default;
		virtual ~IApplication() = default;

		virtual void OnUpdate(Timestep timestep) = 0;
		virtual void OnEvent(Event& event) = 0;

		virtual bool OnKeyEvent(Ethereal::KeyEvent& event) = 0;
	};
}