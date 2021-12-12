#pragma once
#include <memory>
#include <vector>

namespace Ethereal
{

	enum class FramebufferTextureFormat
	{
		None = 0,

		//Color
		RGBA8,
		RED_INTEGER,
		
		//Depth/stencil
		DEPTH24STENCIL8,

		//Defaults
		Depth = DEPTH24STENCIL8
	};

	struct FramebufferTextureSpecification
	{
		FramebufferTextureSpecification() = default;
		FramebufferTextureSpecification(FramebufferTextureFormat format)
			: TextureFormat(format)
		{}

		FramebufferTextureFormat TextureFormat = FramebufferTextureFormat::None;
		//TODO: filtering/wrap
	};

	struct FramebufferAttachmentSpecification
	{
		FramebufferAttachmentSpecification() = default;
		FramebufferAttachmentSpecification(std::initializer_list<FramebufferTextureSpecification> attachments)
			:Attachments(attachments)
		{}

		std::vector<FramebufferTextureSpecification> Attachments;
	};

	struct FramebufferSpecification
	{
		uint32_t width, height, samples = 1;
		FramebufferAttachmentSpecification Attachments;

	};

	class Framebuffer
	{
	public:
		Framebuffer(const FramebufferSpecification& spec);
		~Framebuffer();

		void Use() const;
		void Unuse() const;

		void Resize(uint32_t width, uint32_t height);
		int ReadPixel(uint32_t attachment, int x, int y);

		void ClearColorAttachment(uint32_t index, const int value);

		uint32_t GetFramebuffer() const;
		uint32_t GetColorAttachment(uint32_t index = 0) const;

		const FramebufferSpecification& GetSpecification() const;

		static std::unique_ptr<Framebuffer> Create(const FramebufferSpecification& spec);
	private:
		void Invalidate();
	private:
		FramebufferSpecification specification;

		uint32_t framebufferID;

		std::vector<FramebufferTextureSpecification> colorAttachmentSpecs;
		FramebufferTextureSpecification depthAttachmentSpec = FramebufferTextureFormat::None;

		std::vector<uint32_t> colorAttachments;
		uint32_t depthAttachment = 0;
	};
}

