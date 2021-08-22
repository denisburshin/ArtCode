#include <Ethereal/Events/ApplicationEvent.h>

namespace Ethereal
{
	std::string CloseEvent::GetName() const
	{
		return "Close event";
	}

	EventType CloseEvent::GetStaticType()
	{
		return EventType::Close;
	}

	EventType CloseEvent::GetType() const
	{
		return GetStaticType();
	}
}