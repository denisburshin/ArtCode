#pragma once
#include <Core/Layer.h>
#include <Core/Events/MouseEvent.h>

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


