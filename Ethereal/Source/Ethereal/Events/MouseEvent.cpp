#include <Ethereal/Events/MouseEvent.h>
#include <sstream>

namespace Ethereal
{

	MouseEvent::MouseEvent(float x, float y)
		: xPos(x), yPos(y)
	{

	}

	std::string MouseEvent::GetName() const
	{
		std::stringstream ss;
		ss << "Mouse event -> " << xPos << ", " << yPos;
		return ss.str();
	}

	EventType MouseEvent::GetStaticType()
	{
		return EventType::MouseMove;
	}

	EventType MouseEvent::GetType() const
	{
		return GetStaticType();
	}

	std::pair<double, double> MouseEvent::GetCursorPosition() const
	{
		return { xPos, yPos };
	}

};