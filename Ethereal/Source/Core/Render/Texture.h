#pragma once
#include <memory>
#include <string>

namespace Ethereal
{
	class Texture
	{
	public:
		virtual void Bind() const = 0;
	};

	class Texture2D : public Texture
	{
	public:
		Texture2D(const std::string& path);
		~Texture2D();

		void Bind() const override;

		static std::unique_ptr<Texture2D> Create(const std::string& path);

	private:
		uint32_t width, height, id;
	};
}


