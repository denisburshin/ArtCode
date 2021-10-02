#pragma once
#include <glm/glm.hpp>
#include <Core/Render/Buffer.h>

namespace Ethereal
{
	class Model
	{
	public:
		Model();
		~Model();
	private:
		std::shared_ptr<VertexArray> vao;
		std::shared_ptr<VertexBuffer> vbo;
		std::shared_ptr<IndexBuffer> ibo;

		glm::vec3 transform, scale, rotate;
	};
}

