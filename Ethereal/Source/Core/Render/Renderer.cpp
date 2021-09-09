#include <Core/Render/Renderer.h>

#include <glad/gl.h>

namespace Ethereal
{
	Renderer::SceneData* Renderer::sceneData = new Renderer::SceneData;

	Renderer::Statistics* Renderer::stats = new Renderer::Statistics;

	void Renderer::BeginScene(OrthographicCamera& camera)
	{
		sceneData->ProjectionViewMatrix = camera.GetProjectionMatrix() * camera.GetViewMatrix();
	}

	void Renderer::Submit(const std::shared_ptr<VertexArray>& vertexArray, const std::shared_ptr<Shader>& program,
		const glm::mat4& transform)
	{
		program->Use();
		program->UploadUniformMat4("u_ProjectionView", sceneData->ProjectionViewMatrix);
		program->UploadUniformMat4("u_ModelMatrix", transform);	
		vertexArray->Bind();

		DrawIndexed(vertexArray);
	}

	void Renderer::EndScene()
	{
	}

	void Renderer::DrawIndexed(const std::shared_ptr<VertexArray>& vertexArray)
	{
		glDrawElements(GL_TRIANGLES, vertexArray->GetIndexBuffer()->GetCount(), GL_UNSIGNED_INT, nullptr);

		++stats->drawCalls;
	}

	void Renderer::SetClearColor(const glm::vec4& color)
	{
		glClearColor(color.r, color.g, color.b, color.a);
	}

	void Renderer::SetViewport(uint32_t width, uint32_t height)
	{
		glViewport(0, 0, width, height);
	}

	void Renderer::ClearColor()
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}

	uint32_t Renderer::DrawCalls()
	{
		return stats->drawCalls;
	}

	void Renderer::ResetStatistic()
	{
		stats->drawCalls = 0;
	}
};
