#pragma once
#include <string>
#include <vector>
#include <memory>

namespace Ethereal
{
	enum class ShaderDataType
	{
		Float, Vec2f, Vec3f, Vec4f, Mat3f, Mat4f, Int, Vec2i, Vec3i, Vec4i, Bool
	};

	enum class DataType
	{
		Single = 1, Vec2, Vec3, Vec4, Mat3 = 3 * 3, Mat4 = 4 * 4
	};

	uint32_t ShaderDataTypeSize(ShaderDataType type);

	uint32_t ShaderDataTypeToBase(ShaderDataType type);

	struct BufferElement
	{
		std::string _name;
		unsigned int _size;
		ShaderDataType _type;
		unsigned int offset;
		bool _normalized;

		BufferElement(ShaderDataType type, const std::string& name, bool normalized = false);

		unsigned int GetComponentCount() const;
	};

	class BufferLayout
	{
	public:

		BufferLayout();
		BufferLayout(const std::initializer_list<BufferElement>& elements);

		unsigned int GetStride() const;

		std::vector<BufferElement>::iterator begin();
		std::vector<BufferElement>::iterator end();
	private:
		void CalculateOffset();

		std::vector<BufferElement> _elements;
		unsigned int stride;
	};

	class VertexBuffer
	{
	public:

		VertexBuffer(uint32_t size);
		VertexBuffer(const float* vertices, const size_t size);
		~VertexBuffer();

		void Bind() const;
		void Unbind() const;

		static VertexBuffer* Create(uint32_t size);
		static VertexBuffer* Create(float* vertices, size_t size);
		static VertexBuffer* Create(const std::vector<float>& vertices);

		void SetData(const void* data, uint32_t size);

		void SetLayout(const BufferLayout& layout);
		const BufferLayout& GetLayout() const;

	private:
		unsigned int VBO;

		BufferLayout layout;
	};

	class IndexBuffer
	{
	public:

		IndexBuffer(uint32_t* indices, uint32_t size);

		~IndexBuffer();

		void Bind() const;
		void Unbind() const;

		unsigned int GetCount() const;

		static IndexBuffer* Create(uint32_t* indices, uint32_t size);

	private:
		unsigned int IBO;
		uint32_t count;
	};

	class VertexArray
	{
	public:

		VertexArray();

		void Bind() const;
		void Unbind() const;

		void AddVertexBuffer(const std::shared_ptr<VertexBuffer>& vBuffer);
		void AddIndexBuffer(const std::shared_ptr<IndexBuffer>& iBuffer);

		const std::shared_ptr<IndexBuffer>& GetIndexBuffer() const;

		static VertexArray* Create();

	private:
		unsigned int VAO;
		std::vector<std::shared_ptr<VertexBuffer>> vBuffers;
		std::shared_ptr<IndexBuffer> indexBuffer;
	};
};


