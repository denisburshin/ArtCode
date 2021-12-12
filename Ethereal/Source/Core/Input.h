#pragma once
#include <Core/Events/KeyCodes.h>
#include <glm/vec2.hpp>

namespace Ethereal
{
	class Input
	{
	public:
		static bool IsKeyPressed(Key key);

		static bool IsMouseButtonPressed(Mouse button);
		static glm::vec2 GetMousePosition();
		static float GetMouseX();
		static float GetMouseY();

	private:
		static Input* instance;
	};
}

