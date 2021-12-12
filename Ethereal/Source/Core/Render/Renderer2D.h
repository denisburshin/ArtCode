#pragma once
#include <Core/Render/Camera.h>
#include <Core/Render/OrthographicCamera.h>
#include <Core/Render/Texture.h>
#include <Core/Render/EditorCamera.h>
#include <Core/Scene/Components.h>

namespace Ethereal {
	class Renderer2D
	{
	public:
		static void Init();

#pragma region OLD RENDER PATH
	#if 0		
			static void BeginScene(const OrthographicCamera& camera);

			static void DrawQuad(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color);
			static void DrawQuad(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color);
			static void DrawQuad(const glm::vec2& position, const glm::vec2& size, std::shared_ptr<Texture2D>& texture);
			static void DrawQuad(const glm::vec3& position, const glm::vec2& size, std::shared_ptr<Texture2D>& texture);
			static void DrawRotatedQuad(const glm::vec2& position, const glm::vec2& size, float rotation, std::shared_ptr<Texture2D>& texture);
			static void DrawRotatedQuad(const glm::vec3& position, const glm::vec2& size, float rotation, std::shared_ptr<Texture2D>& texture);
	#endif
#pragma endregion

		static void BeginScene(const EditorCamera& camera);
		static void BeginScene(const Camera& camera, const glm::mat4& transform);
		static void DrawEntity(const glm::mat4& transform, const SpriteComponent& sc, int entity);

		static void DrawQuad(const glm::mat4& transform, const std::shared_ptr<Texture2D>& texture, const glm::vec4& color, const float tiling, int entity = -1);
		static void DrawQuad(const glm::mat4& transform, const glm::vec4& color, int entity = -1);

		
		static void EndScene();
		
		static void Shutdown();
	};
}

