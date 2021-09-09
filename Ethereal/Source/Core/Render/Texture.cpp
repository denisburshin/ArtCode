#include <Core/Render/Texture.h>
#include <stbimage.h>

#include <glad/gl.h>

namespace Ethereal
{
	Texture2D::Texture2D(const std::string& path)
	{
		int w, h, channels;
		stbi_set_flip_vertically_on_load(1);
		auto data = stbi_load(path.c_str(), &w, &h, &channels, 0);

		width = w;
		height = h;

		glCreateTextures(GL_TEXTURE_2D, 1, &id);
		glTextureStorage2D(id, 1, GL_RGB8, width, height);

		glTextureParameteri(id, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTextureParameteri(id, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		glTextureSubImage2D(id, 0, 0, 0, width, height, GL_RGB, GL_UNSIGNED_BYTE, data);

		stbi_image_free(data);
	}

	Texture2D::~Texture2D()
	{
		glDeleteTextures(1, &id);
	}

	void Texture2D::Bind() const
	{
		glBindTextureUnit(0, id);
	}

	std::unique_ptr<Texture2D> Texture2D::Create(const std::string& path)
	{
		return std::make_unique<Texture2D>(path);
	}

}