#pragma once
#include <Core/Render/Shader.h>
#include <Core/Render/Buffer.h>
#include <Core/Render/OrthographicCamera.h>

namespace Ethereal
{
	class Renderer
	{
	public:
		static void BeginScene(OrthographicCamera& camera);

		static void Submit(const std::shared_ptr<VertexArray>& vertexArray, const std::shared_ptr<Shader>& program,
			const glm::mat4& transform = glm::mat4(1.0f));

		static void EndScene();

		static void DrawIndexed(const std::shared_ptr<VertexArray>& vertexArray);

		static void SetClearColor(const glm::vec4& color);

		static void SetViewport(uint32_t width, uint32_t height);

		static void ClearColor();

		static uint32_t DrawCalls();

		static void ResetStatistic();

	private:

		struct SceneData
		{
			glm::mat4 ProjectionViewMatrix;
		};

		struct Statistics
		{
			uint32_t drawCalls = 0;
		};

		static SceneData* sceneData;
		static Statistics* stats;
	};
};


