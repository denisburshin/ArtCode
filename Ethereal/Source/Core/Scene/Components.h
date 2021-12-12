#pragma once

#include <Core/Scene/SceneCamera.h>
#include <Core/Scene/ScriptableEntity.h>

#include <glm/gtx/quaternion.hpp>
#include <Core/Render/Texture.h>

namespace Ethereal
{
	struct TagComponent
	{
		std::string tag;

		TagComponent() = default;
		TagComponent(const TagComponent&) = default;
		TagComponent(const std::string& tag)
			: tag(tag)
		{}
	};

	struct CameraComponent
	{
		SceneCamera camera;
		bool primary = true;
		bool fixedAspectRatio = false;

		CameraComponent() = default;
		CameraComponent(const CameraComponent&) = default;
	};

	struct TransformComponent
	{
		glm::vec3 Translation{ 0.0f, 0.0f, 0.0f };
		glm::vec3 Rotation{ 0.0f, 0.0f, 0.0f };
		glm::vec3 Scale{ 1.0f, 1.0f, 1.0f };

		TransformComponent() = default;
		TransformComponent(const TransformComponent&) = default;
		TransformComponent(const glm::vec3& translation)
			: Translation(translation) {}

		glm::mat4 GetTransform()
		{
			glm::mat4 rotation = glm::toMat4(glm::quat(Rotation));

			return glm::translate(glm::mat4(1.0f), Translation)
				* rotation * glm::scale(glm::mat4(1.0f), Scale);
		}
	};

	struct SpriteComponent
	{
		glm::vec4 color{ 1.0f, 1.0f, 1.0f, 1.0f };
		std::shared_ptr<Texture2D> texture;

		float tilingFactor = 1.0f; // TODO: Implement in Renderer

		SpriteComponent() = default;
		SpriteComponent(const SpriteComponent&) = default;
		SpriteComponent(const glm::vec4& color)
			: color(color) {}
	};

	struct NativeScriptComponent
	{
		ScriptableEntity* instance = nullptr;

		ScriptableEntity* (*InstantiateScript)();
		void (*DestroyScript)(NativeScriptComponent*);
		 
		template <class T>
		void Bind()
		{
			InstantiateScript = []() { return static_cast<ScriptableEntity*>(new T); }
			DestroyScript = [](NativeScriptComponent* nsc) {delete nsc->instance; nsc->instance = nullptr; }
		}
	};
}