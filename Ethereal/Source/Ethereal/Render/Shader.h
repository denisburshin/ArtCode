#pragma once
#include <string>
#include <vector>
#include <glm/glm.hpp>

namespace Ethereal
{
	class Shader
	{
	public:
		Shader(const std::string& vertexCode, const std::string& fragmentCode);
		void UploadUniformMat4(const std::string& uniform, const glm::mat4& matrix);

		uint32_t GetHandler() const;
		void Use() const;
		void Unuse() const;
	private:
		uint32_t program;
	};
}

