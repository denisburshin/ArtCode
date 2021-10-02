#pragma once
#include <memory>

namespace Ethereal
{
	struct FramebufferSpecification
	{
		uint32_t width, height;
	};

	class Framebuffer
	{
	public:
		Framebuffer(const FramebufferSpecification& spec);
		~Framebuffer();

		void Use() const;
		void Unuse() const;

		void Resize(uint32_t width, uint32_t height);

		uint32_t GetFramebuffer() const;
		uint32_t GetColorAttachment() const;

		static std::unique_ptr<Framebuffer> Create(const FramebufferSpecification& spec);
	private:
		void Invalidate();
	private:
		FramebufferSpecification specification;

		uint32_t framebufferID;
		uint32_t colorAttachmentID, depthAttachmentID; 
	};
}

