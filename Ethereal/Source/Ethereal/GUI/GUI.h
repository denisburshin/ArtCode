#pragma once

#include <Ethereal/IApplication.h>

namespace Ethereal
{
	class GUI : public IApplication
	{
	public:
		GUI();
		virtual ~GUI();

		void OnUpdate() override;
		void OnEvent(Event& event) override;
		virtual bool OnKeyEvent(Ethereal::KeyEvent& event) override;
	};
}


