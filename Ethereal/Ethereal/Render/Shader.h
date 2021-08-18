#pragma once
#include <glad/gl.h>
#include <string>
#include <vector>
#include <glm/glm.hpp>

namespace Ethereal
{
	enum class Type
	{
		Vertex = GL_VERTEX_SHADER,
		Fragment = GL_FRAGMENT_SHADER,
		Geometry = GL_GEOMETRY_SHADER
	};

	class Shader
	{
	public:
		Shader(const std::string& file, Type type);
		GLuint GetHandler() const;
	private:
		GLuint hShader;
	};

	class Program
	{
	public:
		Program(const std::vector<Shader>& shaderList);
		~Program();

		void UploadUniformMat4(const std::string& uniform, const glm::mat4& matrix);

		GLuint GetHandler() const;
		void Use() const;
		void Unuse() const;
	private:
		GLuint hProgram;
		std::vector<Shader> shaders;
	};

}

