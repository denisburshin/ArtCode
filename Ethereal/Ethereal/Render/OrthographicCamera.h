#pragma once
#include <glm/glm.hpp>

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

		const glm::mat4& GetProjectionMatrix() const;
		const glm::mat4& GetViewMatrix() const;

	private:
		void RecalculateViewMatrix();

		glm::mat4 projectionMatrix;
		glm::mat4 viewMatrix;

		glm::vec3 position;
		float rotation;
	};
};


