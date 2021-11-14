#pragma once
#include <Core/Render/Camera.h>
#include <glm/gtc/matrix_transform.hpp>

namespace Ethereal
{
	class SceneCamera : public Camera
	{
	public:
		enum class ProjectionType { Perspective = 0, Orthographic = 1};
	public:
		SceneCamera();
		virtual ~SceneCamera() = default;

		void SetOrthographic(float size, float near, float far);
		void SetPerspective(float fov, float nearClip, float farClip);
		void SetViewportSize(uint32_t width, uint32_t height);

		ProjectionType GetProjectionType() const;

		float GetOrthoNearClip() const;
		float GetOrhtoFarClip() const;
		float GetOrhtoSize() const;
		void SetOrthoNearClip(float nearClip);
		void SetOrthoFarClip(float farClip);
		void SetOrthoSize(float size);

		float GetPerspectiveNearClip() const;
		float GetPerspectiveFarClip() const;
		float GetFOV() const;
		void SetPerspectiveNearClip(float nearClip);
		void SetPerspectiveFarClip(float farClip);
		void SetFOV(float fov);

		void SetProjectionType(ProjectionType type);
	private:
		void RecalculateProjection();
	private:
		ProjectionType projectionType = ProjectionType::Orthographic;
		float orthoSize, orthoNear, orthoFar;

		float perspectiveFOV, perspectiveNear, perspectiveFar;
		float aspectRatio;

	};
}

