#pragma once
#include <Ethereal/IApplication.h>
#include <Ethereal/Events/MouseEvent.h>

namespace Ethereal
{
	class GUI
	{ 
	public:
		GUI();
		~GUI();

		void OnEvent(Event& event) const;

		void Begin();
		void End();
	};
}


