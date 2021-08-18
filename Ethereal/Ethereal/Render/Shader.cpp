#include <fstream>
#include <iostream>

#include "Shader.h"

#include <glm/gtc/type_ptr.hpp>

namespace Ethereal
{
	Shader::Shader(const std::string& file, Type type)
	{
		std::ifstream in(file);

		if (!in.is_open())
			std::cout << "Shader file not open" << std::endl;

		std::string code;
		code.assign(std::istreambuf_iterator<char>(in.rdbuf()), std::istreambuf_iterator<char>());

		in.close();

		hShader = glCreateShader(static_cast<GLenum>(type));

		const auto s_code = code.c_str();

		glShaderSource(hShader, 1, &s_code, nullptr);
		glCompileShader(hShader);

		GLint result;
		glGetShaderiv(hShader, GL_COMPILE_STATUS, &result);

		if (!result)
		{
			char buffer[1024];
			glGetShaderInfoLog(hShader, 1024, nullptr, buffer);
			std::cout << "ERROR: " << buffer << std::endl;
			glDeleteShader(hShader);
		}

		std::cout << "Shader loaded from file..." << std::endl;
	}

	GLuint Shader::GetHandler() const
	{
		return hShader;
	}

	Program::Program(const std::vector<Shader>& shaderList)
		: shaders(shaderList)
	{
		hProgram = glCreateProgram();

		for (auto& shader : shaders)
			glAttachShader(hProgram, shader.GetHandler());

		glLinkProgram(hProgram);

		GLint isLinked = 0;
		glGetProgramiv(hProgram, GL_LINK_STATUS, (int*)&isLinked);
		if (isLinked == GL_FALSE)
		{
			GLint maxLength = 0;
			glGetProgramiv(hProgram, GL_INFO_LOG_LENGTH, &maxLength);

			// The maxLength includes the NULL character
			std::vector<GLchar> infoLog(maxLength);
			glGetProgramInfoLog(hProgram, maxLength, &maxLength, &infoLog[0]);

			// We don't need the program anymore.
			glDeleteProgram(hProgram);
			// Don't leak shaders either.
			for (auto& shader : shaders)
				glDeleteShader(shader.GetHandler());

			// Use the infoLog as you see fit.
			// TO DO: error handle
			// In this simple program, we'll just leave
		}

		// Always detach shaders after a successful link.
		for (auto& shader : shaders)
			glDetachShader(hProgram, shader.GetHandler());
	}

	Program::~Program()
	{
		glDeleteProgram(hProgram);
	}

	GLuint Program::GetHandler() const
	{
		return hProgram;
	}

	void Program::Use() const
	{
		glUseProgram(hProgram);
	}
	void Program::Unuse() const
	{
		glUseProgram(0);
	}

	void Program::UploadUniformMat4(const std::string& uniform, const glm::mat4& matrix)
	{
		GLint location = glGetUniformLocation(hProgram, uniform.c_str());
		glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(matrix));
	}
}
