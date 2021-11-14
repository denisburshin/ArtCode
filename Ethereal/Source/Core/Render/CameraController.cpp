#include "CameraController.h"
#include <Core/Events/KeyCodes.h>
#include <Core/Input.h>
#include <iostream>

namespace Ethereal
{
	CameraController::CameraController(float aspectRatio)
		: aspectRatio(aspectRatio), zoom(1.0f), camera(-aspectRatio * zoom, aspectRatio * zoom, -zoom, zoom),
		cameraPosition(glm::vec3(0.0f))
	{

	}

	void CameraController::OnUpdate()
	{
		if (Input::IsKeyPressed(Key::W))
		{
			cameraPosition.y -= 0.1f;
		}
		else if (Input::IsKeyPressed(Key::S))
		{
			cameraPosition.y += 0.1f;
		}
		else if (Input::IsKeyPressed(Key::A))
		{
			cameraPosition.x -= 0.1f;
		}
		else if (Input::IsKeyPressed(Key::D))
		{
			cameraPosition.x += 0.1f;
		}

		camera.SetPosition(cameraPosition);
	}

	void CameraController::OnEvent(Event& e)
	{
		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<MouseScrollEvent>(std::bind(&CameraController::OnMouseScrollEvent, this, std::placeholders::_1));
		dispatcher.Dispatch<ResizeEvent>(std::bind(&CameraController::OnWindowResizeEvent, this, std::placeholders::_1));
	}

	bool CameraController::OnMouseScrollEvent(MouseScrollEvent& e)
	{
		zoom -= e.GetYOffset();
		camera.SetProjection(-aspectRatio * zoom, aspectRatio * zoom, -zoom, zoom);
		return true;
	}

	bool CameraController::OnWindowResizeEvent(ResizeEvent& e)
	{
		aspectRatio = (float)e.GetWidth() / (float)e.GetHeight();
		camera.SetProjection(-aspectRatio * zoom, aspectRatio * zoom, -zoom, zoom);
		return true;
	}

	OrthographicCamera& CameraController::GetCamera()
	{
		return camera;
	}

	const OrthographicCamera& CameraController::GetCamera() const
	{
		return camera;
	}
}