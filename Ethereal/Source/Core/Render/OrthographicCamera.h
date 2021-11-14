#pragma once
#include <glm/glm.hpp>
#include <Utility/Time.h>

namespace Ethereal
{
	class OrthographicCamera
	{
	public:
		OrthographicCamera(float left, float right, float bottom, float top);
		~OrthographicCamera() = default;

		void SetPosition(const glm::vec3& position);
		const glm::vec3& GetPosition();

		void SetRotation(float rotation);
		float GetRotation();

		void SetProjection(float left, float right, float bottom, float top);

		const glm::mat4& GetProjectionViewMatrix() const;
		const glm::mat4& GetProjectionMatrix() const;
		const glm::mat4& GetViewMatrix() const;

	private:
		void RecalculateViewMatrix();

		glm::mat4 projectionMatrix;
		glm::mat4 viewMatrix;

		glm::mat4 viewProjectionMatrix;

		glm::vec3 position;
		float rotation;
	};
};


