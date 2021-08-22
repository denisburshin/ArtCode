#pragma once

#include <glad/gl.h>

namespace Ethereal
{
	enum class Type
	{
		Vertex = GL_VERTEX_SHADER,
		Fragment = GL_FRAGMENT_SHADER,
		Geometry = GL_GEOMETRY_SHADER
	};
}
