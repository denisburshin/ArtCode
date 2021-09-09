#pragma once

#include <Core/Events/KeyEvent.h>

namespace Ethereal
{
	class Layer
	{
	public:
		Layer() = default;
		virtual ~Layer() = default;

		virtual void OnUpdate() {};
		virtual void OnEvent(Event& event) = 0;

		virtual bool OnKeyEvent(KeyEvent& event) = 0;

		virtual void OnGUIRender() {};

		virtual std::unique_ptr<Layer> GetLayer() const = 0;
	};
}