#pragma once

#include "Events/EventSystem.h"
#include "Events/KeyEvent.h"
#include "Timestep.h"

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