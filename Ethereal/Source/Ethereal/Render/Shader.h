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
		void UploadUniformVec3(const std::string& uniform, const glm::vec3& vector);
		void UploadUniformInt(const std::string& uniform, const int value);

		void Use() const;
		void Unuse() const;

		uint32_t GetHandler() const;
	private:
		uint32_t program;
	};
}

