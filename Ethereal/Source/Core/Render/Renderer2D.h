#pragma once
#include <Core/Render/Camera.h>
#include <Core/Render/OrthographicCamera.h>
#include <Core/Render/Texture.h>

namespace Ethereal {
	class Renderer2D
	{
	public:
		static void Init();
		static void Shutdown();

		static void BeginScene(const Camera& camera, const glm::mat4& transform);
		static void BeginScene(const OrthographicCamera& camera);
		static void EndScene();

		static void DrawQuad(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color);
		static void DrawQuad(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color);

		static void DrawQuad(const glm::mat4& transform, const glm::vec4& color);
		static void DrawQuad(const glm::mat4& transform, std::shared_ptr<Texture2D>& texture);

		static void DrawQuad(const glm::vec2& position, const glm::vec2& size, std::shared_ptr<Texture2D>& texture);
		static void DrawQuad(const glm::vec3& position, const glm::vec2& size, std::shared_ptr<Texture2D>& texture);

		static void DrawRotatedQuad(const glm::vec2& position, const glm::vec2& size, float rotation, std::shared_ptr<Texture2D>& texture);

		static void DrawRotatedQuad(const glm::vec3& position, const glm::vec2& size, float rotation, std::shared_ptr<Texture2D>& texture);
	};
}

