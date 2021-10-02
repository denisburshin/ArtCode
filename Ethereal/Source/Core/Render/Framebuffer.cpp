#include <Core/Render/Framebuffer.h>
#include <cassert>
#include <glad/gl.h>
#include <iostream>
namespace Ethereal
{
	Framebuffer::Framebuffer(const FramebufferSpecification& spec)
		: specification(spec)
	{
		Invalidate();
	}

	Framebuffer::~Framebuffer()
	{
		glDeleteFramebuffers(1, &framebufferID);
		glDeleteTextures(1, &colorAttachmentID);
		glDeleteTextures(1, &depthAttachmentID);
	}

	void Framebuffer::Use() const
	{
		glBindFramebuffer(GL_FRAMEBUFFER, framebufferID);
		glViewport(0, 0, specification.width, specification.height);
	}

	void Framebuffer::Unuse() const
	{
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	void Framebuffer::Resize(uint32_t width, uint32_t height)
	{
		if (specification.width != width && specification.height != height)
		{
			specification = { width, height };

			Invalidate();
		}
	}

	uint32_t Framebuffer::GetFramebuffer() const
	{
		return framebufferID;
	}

	uint32_t Framebuffer::GetColorAttachment() const
	{
		return colorAttachmentID;
	}

	std::unique_ptr<Framebuffer> Framebuffer::Create(const FramebufferSpecification& spec)
	{
		return std::make_unique<Framebuffer>(spec);
	}

	void Framebuffer::Invalidate()
	{
		if (framebufferID)
		{
			glDeleteFramebuffers(1, &framebufferID);
			glDeleteTextures(1, &colorAttachmentID);
			glDeleteTextures(1, &depthAttachmentID);
		}

		glCreateFramebuffers(1, &framebufferID);
		glBindFramebuffer(GL_FRAMEBUFFER, framebufferID);

		glCreateTextures(GL_TEXTURE_2D, 1, &colorAttachmentID);
		glBindTexture(GL_TEXTURE_2D, colorAttachmentID);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, specification.width,
			specification.height, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, colorAttachmentID, 0);

		glCreateTextures(GL_TEXTURE_2D, 1, &depthAttachmentID);
		glBindTexture(GL_TEXTURE_2D, depthAttachmentID);

		glTexStorage2D(GL_TEXTURE_2D, 1, GL_DEPTH24_STENCIL8, specification.width, specification.height);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_TEXTURE_2D, depthAttachmentID, 0);

		assert(glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE);

		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}
};