#include <Ethereal/Events/KeyEvent.h>

namespace Ethereal
{
	KeyEvent::KeyEvent(int key)
		: key(key)
	{

	}

	std::string KeyEvent::GetName() const
	{
		return "Key Pressed: " + key;
	}

	int KeyEvent::GetKey() const
	{
		return key;
	}

	EventType KeyEvent::GetStaticType()
	{
		return EventType::KeyPressed;
	}

	EventType KeyEvent::GetType() const
	{
		return GetStaticType();
	}


}
