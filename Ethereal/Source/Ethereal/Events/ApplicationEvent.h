#pragma once
#include <Ethereal/Events/EventSystem.h>

namespace Ethereal
{
	class CloseEvent : public Event
	{
	public:
		CloseEvent() = default;
		~CloseEvent() = default;

		std::string GetName() const override;
		
		static EventType GetStaticType();

		EventType GetType() const override;
	};
}