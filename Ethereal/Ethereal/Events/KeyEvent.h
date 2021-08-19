#pragma once
#include "EventSystem.h"

namespace Ethereal
{
	class KeyEvent : public Event
	{
	public:
		KeyEvent(int key);

		std::string GetName() const override;
		int GetKey() const;
		EventType GetType() const override;

		static EventType GetStaticType();


	private:
		int key;
	};
}


