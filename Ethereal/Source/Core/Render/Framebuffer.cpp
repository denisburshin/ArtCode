#include <Core/Render/Framebuffer.h>
#include <cassert>
#include <glad/gl.h>
#include <iostream>

namespace Ethereal
{
	static GLenum TextureTarget(bool multisampled)
	{
		return multisampled ? GL_TEXTURE_2D_MULTISAMPLE : GL_TEXTURE_2D;
	}
	static void CreateTextures(bool multisampled, uint32_t* outID, uint32_t count)
	{
		glCreateTextures(TextureTarget(multisampled), count, outID);
	}
	static void BindTexture(bool multisampled, uint32_t id)
	{
		glBindTexture(TextureTarget(multisampled), id);
	}
	static void AttachDepthTexture(uint32_t id, int samples, GLenum format, GLenum attachmentType, uint32_t width, uint32_t height)
	{
		bool multisampled = samples > 1;
		if (multisampled)
		{
			glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, samples, format, width, height, GL_FALSE);
		}
		else
		{
			glTexStorage2D(GL_TEXTURE_2D, 1, format, width, height);

			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		}

		glFramebufferTexture2D(GL_FRAMEBUFFER, attachmentType, TextureTarget(multisampled), id, 0);
	}

	static GLenum TextureFormatToInternal(FramebufferTextureFormat format)
	{
		switch (format)
		{
		case FramebufferTextureFormat::RGBA8:
			return GL_RGBA8;
		case FramebufferTextureFormat::RED_INTEGER:
			return GL_RED_INTEGER;
		default:
			return 0;
		}
	}

	static void AttachColorTexture(uint32_t id, int samples, GLenum internalFormat, GLenum format, uint32_t width, uint32_t height, int index)
	{
		bool multisampled = samples > 1;
		if (multisampled)
		{
			glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, samples, internalFormat, width, height, GL_FALSE);
		}
		else
		{
			glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, width, height, 0, format, GL_UNSIGNED_BYTE, nullptr);

			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		}

		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + index, TextureTarget(multisampled), id, 0);

	}

	static bool IsDepthFormat(FramebufferTextureFormat format)
	{
		switch (format)
		{
		//case Ethereal::FramebufferTextureFormat::None:
		//	break;
		//case Ethereal::FramebufferTextureFormat::RGBA8:
		//	break;
		case Ethereal::FramebufferTextureFormat::DEPTH24STENCIL8:
			return true;
		//case Ethereal::FramebufferTextureFormat::Depth:
		//	break;
		//default:
		//	break;
		}
		return false;
	}
	Framebuffer::Framebuffer(const FramebufferSpecification& spec)
		: specification(spec)
	{

		for (auto spec : specification.Attachments.Attachments)
		{
			if (!IsDepthFormat(spec.TextureFormat))
			{
				colorAttachmentSpecs.emplace_back(spec);
			}
			else
			{
				depthAttachmentSpec = spec;
			}
		}
		Invalidate();
	}

	Framebuffer::~Framebuffer()
	{
		glDeleteFramebuffers(1, &framebufferID);
		glDeleteTextures(colorAttachments.size(), colorAttachments.data());
		glDeleteTextures(1, &depthAttachment);
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

	int Framebuffer::ReadPixel(uint32_t attachment, int x, int y)
	{
		glReadBuffer(GL_COLOR_ATTACHMENT0 + attachment);
		int pixelData;
		glReadPixels(x, y, 1, 1, GL_RED_INTEGER, GL_INT, &pixelData);
		return pixelData;
	}

	void Framebuffer::ClearColorAttachment(uint32_t index, int value)
	{
		auto& tf = colorAttachmentSpecs[index].TextureFormat;

		glClearTexImage(colorAttachments[index], 0, TextureFormatToInternal(tf), GL_INT, &value);
	}


	uint32_t Framebuffer::GetFramebuffer() const
	{
		return framebufferID;
	}

	uint32_t Framebuffer::GetColorAttachment(uint32_t index) const
	{
		return colorAttachments[index];
	}

	const FramebufferSpecification& Framebuffer::GetSpecification() const
	{
		return specification;
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
			glDeleteTextures(colorAttachments.size(), colorAttachments.data());
			glDeleteTextures(1, &depthAttachment);

			colorAttachments.clear();
			depthAttachment = 0;
		}

		glCreateFramebuffers(1, &framebufferID);
		glBindFramebuffer(GL_FRAMEBUFFER, framebufferID);

		bool multisample = specification.samples > 1;

		if (colorAttachmentSpecs.size())
		{
			colorAttachments.resize(colorAttachmentSpecs.size());
			CreateTextures(multisample, colorAttachments.data(), colorAttachments.size());
			//Attachments
			for (auto i = 0; i < colorAttachments.size(); ++i)
			{
				BindTexture(multisample, colorAttachments[i]);
				switch (colorAttachmentSpecs[i].TextureFormat)
				{
				case FramebufferTextureFormat::RGBA8:
					AttachColorTexture(colorAttachments[i], specification.samples, GL_RGBA8, GL_RGBA, specification.width, specification.height, i);
					break;
				case FramebufferTextureFormat::RED_INTEGER:
					AttachColorTexture(colorAttachments[i], specification.samples, GL_R32I, GL_RED_INTEGER, specification.width, specification.height, i);
					break;
				default:
					break;
				}
			}
		}

		if (depthAttachmentSpec.TextureFormat != FramebufferTextureFormat::None)
		{
			CreateTextures(multisample, &depthAttachment, 1);
			BindTexture(multisample, depthAttachment);
			switch (depthAttachmentSpec.TextureFormat)
			{
			case FramebufferTextureFormat::DEPTH24STENCIL8:
				AttachDepthTexture(depthAttachment, specification.samples, GL_DEPTH24_STENCIL8, GL_DEPTH_STENCIL_ATTACHMENT,
					specification.width, specification.height);
				break;
			default:
				break;
			}
		}

		if (colorAttachments.size() > 1)
		{
			GLenum buffers[4] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2, GL_COLOR_ATTACHMENT3 };
			glDrawBuffers(colorAttachments.size(), buffers);
		}
		else if (colorAttachments.empty())
		{
			glDrawBuffer(GL_NONE);
		}
		assert(glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE);

		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}
};