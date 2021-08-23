#pragma once
#include <Ethereal/Events/EventSystem.h>

namespace Ethereal
{
	class MouseEvent : public Event
	{
	public:
		MouseEvent(float x, float y);
		virtual ~MouseEvent() = default;

		std::string GetName() const override;

		static EventType GetStaticType();

		EventType GetType() const override;
	private:
		std::pair<double, double> GetCursorPosition() const;
	private:
		float xPos, yPos;
	};

}

