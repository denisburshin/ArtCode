#pragma once
#include "Shader.h"
#include "Buffer.h"
#include "OrthographicCamera.h"

namespace Ethereal
{
	class Renderer
	{
	public:
		static void BeginScene(OrthographicCamera& camera);
		static void Submit(const std::shared_ptr<VertexArray>& vertexArray, const std::shared_ptr<Program>& program);
		static void EndScene();

		static void DrawIndexed(const std::shared_ptr<VertexArray>& vertexArray);

		static void SetClearColor(const glm::vec4& color);

		static void ClearColor();

	private:

		struct SceneData
		{
			glm::mat4 ProjectionViewMatrix;
		};

		static SceneData* sceneData;
	};
};


