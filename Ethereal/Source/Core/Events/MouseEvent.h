#pragma once
#include <Core/Events/EventSystem.h>

namespace Ethereal
{
	class MouseMovedEvent : public Event
	{
	public:
		MouseMovedEvent(float x, float y)
			: xPos(x), yPos(y)
		{

		}
		virtual ~MouseMovedEvent() = default;

		std::string GetName() const override
		{
			return "Mouse move event";
		}

		static EventType GetStaticType()
		{
			return EventType::MouseMove;
		}

		EventType GetType() const override
		{
			return GetStaticType();
		}

		inline float GetX() const
		{
			return xPos;
		}

		inline float GetY() const
		{
			return yPos;
		}
	private:
		std::pair<double, double> GetCursorPosition() const
		{
			return { xPos, yPos };
		}
	private:
		float xPos, yPos;
	};

	class MousePressedEvent : public Event
	{
	public:
		MousePressedEvent(int button, int mods)
			: button(button), mods(mods)
		{

		}

		virtual ~MousePressedEvent() = default;

		static EventType GetStaticType()
		{
			return EventType::MousePressed;
		}

		EventType GetType() const override
		{
			return GetStaticType();
		}

		std::string GetName() const override
		{
			return "Mouse press event";
		}

		inline int GetButton() const
		{
			return button;
		}

	private:
		int button, mods;
	};

	class MouseReleasedEvent : public Event
	{
	public:
		MouseReleasedEvent(int button, int mods)
			: button(button), mods(mods)
		{

		}

		virtual ~MouseReleasedEvent() = default;

		static EventType GetStaticType()
		{
			return EventType::MouseReleased;
		}

		EventType GetType() const override
		{
			return GetStaticType();
		}

		std::string GetName() const override
		{
			return "Mouse release event";
		}

		inline int GetButton() const
		{
			return button;
		}

	private:
		int button, mods;
	};

}

