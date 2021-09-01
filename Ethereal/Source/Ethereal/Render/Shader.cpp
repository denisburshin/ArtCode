#include <fstream>
#include <iostream>

#include <Ethereal/Render/Shader.h>
#include <glad/gl.h>

#include <glm/gtc/type_ptr.hpp>

namespace Ethereal
{
	Shader::Shader(const std::string& vertexCode, const std::string& fragmentCode)
	{
		std::ifstream in(vertexCode);

		if (!in.is_open())
			std::cout << "Shader file not open" << std::endl;

		std::string code;
		code.assign(std::istreambuf_iterator<char>(in.rdbuf()), std::istreambuf_iterator<char>());

		in.close();

		auto vertexShader = glCreateShader(GL_VERTEX_SHADER);

		auto s_code = code.c_str();

		glShaderSource(vertexShader, 1, &s_code, nullptr);
		glCompileShader(vertexShader);

		GLint result;
		glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &result);

		if (!result)
		{
			char buffer[1024];
			glGetShaderInfoLog(vertexShader, 1024, nullptr, buffer);
			std::cout << "ERROR: " << buffer << std::endl;
			glDeleteShader(vertexShader);
		}

		std::cout << "Vertex Shader Compiled!" << std::endl;

		in.open(fragmentCode);

		if (!in.is_open())
			std::cout << "Shader file not open" << std::endl;

		code.assign(std::istreambuf_iterator<char>(in.rdbuf()), std::istreambuf_iterator<char>());

		in.close();

		auto fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

		s_code = code.c_str();

		glShaderSource(fragmentShader, 1, &s_code, nullptr);
		glCompileShader(fragmentShader);

		glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &result);

		if (!result)
		{
			char buffer[1024];
			glGetShaderInfoLog(fragmentShader, 1024, nullptr, buffer);
			std::cout << "ERROR: " << buffer << std::endl;
			glDeleteShader(fragmentShader);
		}

		std::cout << "Fragment Shader Compiled!" << std::endl;

		program = glCreateProgram();

		glAttachShader(program, vertexShader);
		glAttachShader(program, fragmentShader);

		glLinkProgram(program);

		GLint isLinked = 0;
		glGetProgramiv(program, GL_LINK_STATUS, (int*)&isLinked);
		if (isLinked == GL_FALSE)
		{
			GLint maxLength = 0;
			glGetProgramiv(program, GL_INFO_LOG_LENGTH, &maxLength);

			// The maxLength includes the NULL character
			std::vector<GLchar> infoLog(maxLength);
			glGetProgramInfoLog(program, maxLength, &maxLength, &infoLog[0]);

			// We don't need the program anymore.
			glDeleteProgram(program);
			// Don't leak shaders either.
			
			glDeleteShader(vertexShader);
			glDeleteShader(fragmentShader);

			// Use the infoLog as you see fit.
			// TO DO: error handle
			// In this simple program, we'll just leave
		}

		// Always detach shaders after a successful link.
		glDetachShader(program, vertexShader);
		glDetachShader(program, fragmentShader);
	}

	uint32_t Shader::GetHandler() const
	{
		return program;
	}

	void Shader::Use() const
	{
		glUseProgram(program);
	}
	void Shader::Unuse() const
	{
		glUseProgram(0);
	}

	void Shader::UploadUniformMat4(const std::string& uniform, const glm::mat4& matrix)
	{
		GLint location = glGetUniformLocation(program, uniform.c_str());
		glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(matrix));
	}

	void Shader::UploadUniformVec3(const std::string& uniform, const glm::vec3& vector)
	{
		GLint location = glGetUniformLocation(program, uniform.c_str());
		glUniform3fv(location, 1, glm::value_ptr(vector));
	}

	void Shader::UploadUniformInt(const std::string& uniform, const int value)
	{
		GLint location = glGetUniformLocation(program, uniform.c_str());
		glUniform1i(location, value);
	}

}
