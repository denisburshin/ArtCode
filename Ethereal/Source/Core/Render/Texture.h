#pragma once
#include <memory>
#include <string>

namespace Ethereal
{
	class Texture
	{
	public:
		virtual void Bind(const uint32_t) const = 0;
		virtual void Unbind() const = 0;
	};

	class Texture2D : public Texture
	{
	public:
		Texture2D(const uint32_t width, const uint32_t height);
		Texture2D(const std::string& path);
		~Texture2D();

		void Bind(const uint32_t slot = 0) const override;
		void Unbind() const override;

		void SetData(void* data, const uint32_t size);

		static Texture2D* Create(const uint32_t widht, const uint32_t height);
		static Texture2D* Create(const std::string& path);

		bool operator==(const Texture2D& other) const;

	private:
		uint32_t width, height, id;
	};
}


