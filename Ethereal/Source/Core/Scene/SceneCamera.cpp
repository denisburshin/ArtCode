#include "SceneCamera.h"

namespace Ethereal
{
	SceneCamera::SceneCamera()
		: orthoSize(10.f), orthoNear(-1.0f), orthoFar(1.0f), aspectRatio(0.0f),
		perspectiveFOV(glm::radians(45.0f)), perspectiveNear(0.01f), perspectiveFar(1000.0f)
	{
		RecalculateProjection();
	}
	
	void SceneCamera::SetOrthographic(float size, float near, float far)
	{
		projectionType = ProjectionType::Orthographic;
		orthoSize = size;
		orthoNear = near;
		orthoFar = far;
		RecalculateProjection();
	
	}

	void SceneCamera::SetPerspective(float fov, float nearClip, float farClip)
	{
		projectionType = ProjectionType::Perspective;
		perspectiveFOV = fov;
		perspectiveNear = nearClip;
		perspectiveFar = farClip;
		RecalculateProjection();
	}
	
	void SceneCamera::SetViewportSize(uint32_t width, uint32_t height)
	{
		aspectRatio = (float)width / (float)height;
	
		RecalculateProjection();
	}
	
	SceneCamera::ProjectionType SceneCamera::GetProjectionType() const
	{
		return projectionType;
	}

	float SceneCamera::GetOrthoNearClip() const
	{
		return orthoNear;;
	}

	float SceneCamera::GetOrhtoFarClip() const
	{
		return orthoFar;
	}

	float SceneCamera::GetOrhtoSize() const
	{
		return orthoSize;
	}

	void SceneCamera::SetOrthoNearClip(float nearClip)
	{
		orthoNear = nearClip;
		RecalculateProjection();
	}

	void SceneCamera::SetOrthoFarClip(float farClip)
	{
		orthoFar = farClip;
		RecalculateProjection();
	}

	void SceneCamera::SetOrthoSize(float size)
	{
		orthoSize = size;
		RecalculateProjection();
	}

	float SceneCamera::GetPerspectiveNearClip() const
	{
		return perspectiveNear;
	}

	float SceneCamera::GetPerspectiveFarClip() const
	{
		return perspectiveFar;
	}

	float SceneCamera::GetFOV() const
	{
		return perspectiveFOV;
	}

	void SceneCamera::SetPerspectiveNearClip(float nearClip)
	{
		perspectiveNear = nearClip;
		RecalculateProjection();
	}

	void SceneCamera::SetPerspectiveFarClip(float farClip)
	{
		perspectiveFar = farClip;
		RecalculateProjection();
	}

	void SceneCamera::SetFOV(float fov)
	{
		perspectiveFOV = fov;
		RecalculateProjection();
	}

	void SceneCamera::SetProjectionType(ProjectionType type)
	{
		projectionType = type;
		RecalculateProjection();
	}
	
	void SceneCamera::RecalculateProjection()
	{
		if (projectionType == ProjectionType::Orthographic)
		{
			float left = -orthoSize * aspectRatio * 0.5f;
			float right = orthoSize * aspectRatio * 0.5f;
			float bottom = -orthoSize * 0.5f;
			float top = orthoSize * 0.5f;
	
			projectionMatrix = glm::ortho(left, right, bottom, top, orthoNear, orthoFar);
		}
		else
		{
			projectionMatrix = glm::perspective(perspectiveFOV, aspectRatio, perspectiveNear, perspectiveFar);
		}
	}
}
