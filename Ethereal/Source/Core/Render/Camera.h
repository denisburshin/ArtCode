#pragma once
#include <glm/glm.hpp>

namespace Ethereal
{
	class Camera
	{
	public:
		Camera();
		Camera(const glm::mat4& projection);
		virtual ~Camera() = default;

		const glm::mat4& GetProjection() const;
	protected:
		glm::mat4 projectionMatrix;
	};
}

