#include <Core/Render/OrthographicCamera.h>
#include <glm/gtc/matrix_transform.hpp>

namespace Ethereal
{
	OrthographicCamera::OrthographicCamera(float left, float right, float bottom, float top)
		: projectionMatrix(glm::ortho(left, right, bottom, top, -1.0f, 1.0f)), rotation(0.0f),
		viewMatrix(1.0f), position(0.0f)
	{

	}
	void OrthographicCamera::SetPosition(const glm::vec3& position)
	{
		this->position = position;
		RecalculateViewMatrix();
	}
	void OrthographicCamera::SetRotation(float rotation)
	{
		this->rotation = rotation;
		RecalculateViewMatrix();
	}

	void OrthographicCamera::SetProjection(float left, float right, float bottom, float top)
	{
		projectionMatrix = glm::ortho(left, right, bottom, top, -1.0f, 1.0f);
	}

	const glm::mat4& OrthographicCamera::GetProjectionViewMatrix() const
	{
		return viewProjectionMatrix;
	}

	const glm::mat4& OrthographicCamera::GetProjectionMatrix() const
	{
		return projectionMatrix;
	}

	const glm::mat4& OrthographicCamera::GetViewMatrix() const
	{
		return viewMatrix;
	}

	void OrthographicCamera::RecalculateViewMatrix()
	{
		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position) *
			glm::rotate(glm::mat4(1.0f), rotation, glm::vec3(0, 0, 1));

		viewMatrix = glm::inverse(transform);

		viewProjectionMatrix = viewMatrix * projectionMatrix;
	}
}