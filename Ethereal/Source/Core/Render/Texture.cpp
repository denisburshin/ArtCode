#include <Core/Render/Texture.h>
#include <stbimage.h>
#include <glad/gl.h>

namespace Ethereal
{
	Texture2D::Texture2D(const uint32_t width, const uint32_t height)
		: width(width), height(height)
	{
		glCreateTextures(GL_TEXTURE_2D, 1, &id);
		glTextureStorage2D(id, 1, GL_RGBA8, width, height);

		glTextureParameteri(id, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTextureParameteri(id, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		glTextureParameteri(id, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTextureParameteri(id, GL_TEXTURE_WRAP_T, GL_REPEAT);
	}

	Texture2D::Texture2D(const std::string& path)
	{
		int w, h, channels;
		auto data = stbi_load(path.c_str(), &w, &h, &channels, 0);

		width = w;
		height = h;

		GLenum internalFormat = 0, dataFormat = 0;
		if (channels == 4)
		{
			internalFormat = GL_RGBA8;
			dataFormat = GL_RGBA;
		}
		else 
		{
			internalFormat = GL_RGB8;
			dataFormat = GL_RGB;
		}

		glCreateTextures(GL_TEXTURE_2D, 1, &id);
		glTextureStorage2D(id, 1, internalFormat, width, height);

		glTextureParameteri(id, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTextureParameteri(id, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		glTextureParameteri(id, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTextureParameteri(id, GL_TEXTURE_WRAP_T, GL_REPEAT);

		glTextureSubImage2D(id, 0, 0, 0, width, height, dataFormat, GL_UNSIGNED_BYTE, data);

		stbi_image_free(data);
	}

	Texture2D::~Texture2D()
	{
		glDeleteTextures(1, &id);
	}

	void Texture2D::Bind(const uint32_t slot) const
	{
		glBindTextureUnit(slot, id);
	}

	void Texture2D::Unbind() const
	{
		glBindTextureUnit(0, 0);
	}

	void Texture2D::SetData(void* data, const uint32_t size)
	{
		glTextureSubImage2D(id, 0, 0, 0, width, height, GL_RGBA, GL_UNSIGNED_BYTE, data);
	}

	Texture2D* Texture2D::Create(const uint32_t width, const uint32_t height)
	{
		return new Texture2D(width, height);
	}

	Texture2D* Texture2D::Create(const std::string& path)
	{
		return new Texture2D(path);
	}

	bool Texture2D::operator==(const Texture2D& other) const
	{
		return id == other.id;
	}

}