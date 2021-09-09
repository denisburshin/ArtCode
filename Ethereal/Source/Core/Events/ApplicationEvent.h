#pragma once
#include <Core/Events/EventSystem.h>

namespace Ethereal
{
	class CloseEvent : public Event
	{
	public:
		CloseEvent() = default;
		~CloseEvent() = default;

		std::string GetName() const
		{
			return "Close event";
		}

		static EventType GetStaticType()
		{
			return EventType::Close;
		}

		EventType GetType() const
		{
			return GetStaticType();
		}
	};

	class ResizeEvent : public Event
	{
	public:
		ResizeEvent(int w, int h)
			: width(w), height(h)
		{

		}

		~ResizeEvent() = default;

		std::string GetName() const
		{
			return "Window Resize Event";
		}

		static EventType GetStaticType()
		{
			return EventType::Resize;
		}

		int GetWidth() const
		{
			return width;
		}

		int GetHeight() const
		{
			return height;
		}

		EventType GetType() const
		{
			return GetStaticType();
		}
	private:
		int width, height;
	};

}