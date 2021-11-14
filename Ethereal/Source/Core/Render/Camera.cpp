#include "Camera.h"

namespace Ethereal
{
	Camera::Camera()
		: projectionMatrix(glm::mat4(1.0f))
	{ }

	Camera::Camera(const glm::mat4& projection)
		: projectionMatrix(projection)
	{

	}

	const glm::mat4& Camera::GetProjection() const
	{
		return projectionMatrix;
	}


}