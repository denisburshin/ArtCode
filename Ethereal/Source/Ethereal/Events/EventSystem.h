#pragma once
#include <string>
#include <iostream>
#include <functional>

namespace Ethereal
{
	enum class EventType
	{
		Close, ResizeWindow,
		KeyPressed, KeyReleased,
		MouseMove, None
	};

	class Event
	{
	public:
		virtual std::string GetName() const = 0;
		
		virtual EventType GetType() const = 0;

		virtual ~Event() = default;

		bool handled = false;
	};

	class EventDispatcher
	{
	public:
		EventDispatcher(Event& event)
			: hEvent(event)
		{

		}

		template<typename T, typename F>
		bool Dispatch(const F& func)
		{
			if (T::GetStaticType() == hEvent.GetType())
			{
				hEvent.handled = func(static_cast<T&>(hEvent));
				return true;
			}
			return false;
		}

	private:
		Event& hEvent;
	};
}