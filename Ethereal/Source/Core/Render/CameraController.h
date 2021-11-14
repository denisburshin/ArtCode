#pragma once
#include <Core/Render/OrthographicCamera.h>
#include <Core/Events/ApplicationEvent.h>
#include <Core/Events/MouseEvent.h>

namespace Ethereal
{
	class CameraController
	{
	public:
		CameraController(float aspectRatio);

		void OnUpdate();
		void OnEvent(Event& e);

		OrthographicCamera& GetCamera();
		const OrthographicCamera& GetCamera() const;
	private:
		bool OnMouseScrollEvent(MouseScrollEvent& e);
		bool OnWindowResizeEvent(ResizeEvent& e);

	private:
		float aspectRatio, zoom;
		glm::vec3 cameraPosition;
		OrthographicCamera camera;
	};
}


