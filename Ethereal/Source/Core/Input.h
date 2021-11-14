#pragma once
#include <Core/Events/KeyCodes.h>

namespace Ethereal
{
	class Input
	{
	public:
		static bool IsKeyPressed(Key key);
	private:
		static Input* instance;
	};
}

