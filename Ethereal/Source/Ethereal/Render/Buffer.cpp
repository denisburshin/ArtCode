#include <Ethereal/Render/Buffer.h>

#include <glad/gl.h>

namespace Ethereal
{
	VertexBuffer::VertexBuffer(float* vertices, size_t size)
	{
		glCreateBuffers(1, &VBO);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, size, vertices, GL_STATIC_DRAW);
	}

	VertexBuffer::~VertexBuffer()
	{
		glDeleteBuffers(1, &VBO);
	}

	const BufferLayout& VertexBuffer::GetLayout() const
	{
		return layout;
	}

	void VertexBuffer::Bind() const
	{
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
	}

	void VertexBuffer::Unbind() const
	{
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}


	VertexBuffer* VertexBuffer::Create(float* vertices, size_t size)
	{
		return new VertexBuffer(vertices, size);
	}

	void VertexBuffer::SetLayout(const BufferLayout& layout)
	{
		this->layout = layout;
	}

	IndexBuffer::IndexBuffer(unsigned int* indices, size_t size)
		: count(size / sizeof(unsigned int))
	{
		glCreateBuffers(1, &IBO);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, indices, GL_STATIC_DRAW);
	}

	IndexBuffer::~IndexBuffer()
	{
		glDeleteBuffers(1, &IBO);
	}

	void IndexBuffer::Bind() const
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
	}

	void IndexBuffer::Unbind() const
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}

	IndexBuffer* IndexBuffer::Create(unsigned int* indices, size_t size)
	{
		return new IndexBuffer(indices, size);
	}

	unsigned int IndexBuffer::GetCount() const
	{
		return count;
	}

	BufferLayout::BufferLayout(const std::initializer_list<BufferElement>& elements)
		: _elements(elements), stride(0)
	{
		CalculateOffset();
	}

	unsigned int BufferLayout::GetStride() const
	{
		return stride;
	}

	std::vector<BufferElement>::iterator BufferLayout::begin()
	{
		return _elements.begin();
	}

	BufferLayout::BufferLayout()
		: stride(0)
	{

	}

	std::vector<BufferElement>::iterator BufferLayout::end()
	{
		return _elements.end();
	}

	void BufferLayout::CalculateOffset()
	{
		auto offset = 0;
		for (auto& element : _elements)
		{
			element.offset = offset;
			offset += element._size;
			stride += element._size;
		}
	}

	unsigned int ShaderDataTypeSize(ShaderDataType type)
	{
		auto floatSize = sizeof(GL_FLOAT);
		auto intSize = sizeof(GL_INT);
		auto boolSize = sizeof(GL_BOOL);

		switch (type)
		{
		case ShaderDataType::Float: return floatSize;
		case ShaderDataType::Vec2f: return floatSize * 2;
		case ShaderDataType::Vec3f: return floatSize * 3;
		case ShaderDataType::Vec4f: return floatSize * 4;
		case ShaderDataType::Mat3f: return floatSize * 3 * 3;
		case ShaderDataType::Mat4f: return floatSize * 4 * 4;
		case ShaderDataType::Int:   return intSize;
		case ShaderDataType::Vec2i: return intSize * 2;
		case ShaderDataType::Vec3i: return intSize * 3;
		case ShaderDataType::Vec4i: return intSize * 4;
		case ShaderDataType::Mat3i: return intSize * 3 * 3;
		case ShaderDataType::Mat4i: return intSize * 4 * 4;
		case ShaderDataType::Bool:  return boolSize;
		default: return 0;
		}
	}

	GLenum ShaderDataTypeToBase(ShaderDataType type)
	{
		switch (type)
		{
		case ShaderDataType::Float:
		case ShaderDataType::Vec2f:
		case ShaderDataType::Vec3f:
		case ShaderDataType::Vec4f:
		case ShaderDataType::Mat3f:
		case ShaderDataType::Mat4f: return GL_FLOAT;
		case ShaderDataType::Int:
		case ShaderDataType::Vec2i:
		case ShaderDataType::Vec3i:
		case ShaderDataType::Vec4i:
		case ShaderDataType::Mat3i:
		case ShaderDataType::Mat4i: return GL_INT;
		case ShaderDataType::Bool:  return GL_BOOL;
		default: return GL_ZERO;
		}
	}

	BufferElement::BufferElement(ShaderDataType type, const std::string& name, bool normalized)
		: _type(type), _name(name), _size(ShaderDataTypeSize((type))), offset(0), _normalized(normalized)
	{

	}

	unsigned int BufferElement::GetComponentCount() const
	{
		switch (_type)
		{
		case ShaderDataType::Float: return 1;
		case ShaderDataType::Vec2f: return 2;
		case ShaderDataType::Vec3f: return 3;
		case ShaderDataType::Vec4f: return 4;
		case ShaderDataType::Mat3f: return 3 * 3;
		case ShaderDataType::Mat4f: return 4 * 4;
		case ShaderDataType::Int:   return 1;
		case ShaderDataType::Vec2i: return 2;
		case ShaderDataType::Vec3i:	return 3;
		case ShaderDataType::Vec4i: return 4;
		case ShaderDataType::Mat3i: return 3 * 3;
		case ShaderDataType::Mat4i: return 4 * 4;
		case ShaderDataType::Bool:  return 1;
		default: return 0;
		}
	}

	VertexArray::VertexArray()
	{
		glCreateVertexArrays(1, &VAO);
	}

	void VertexArray::Bind() const
	{
		glBindVertexArray(VAO);
	}

	void VertexArray::Unbind() const
	{
		glBindVertexArray(0);
	}

	void VertexArray::AddVertexBuffer(const std::shared_ptr<VertexBuffer>& vBuffer)
	{
		glBindVertexArray(VAO);
		vBuffer->Bind();

		auto index = 0;
		auto layout = vBuffer->GetLayout();
		for (auto& element : layout)
		{
			glEnableVertexAttribArray(index);
			glVertexAttribPointer(index, element.GetComponentCount(),
				ShaderDataTypeToBase(element._type),
				element._normalized,
				layout.GetStride(),
				(const void*)element.offset);
			++index;
		}
		vBuffers.push_back(vBuffer);
	}

	void VertexArray::AddIndexBuffer(const std::shared_ptr<IndexBuffer>& iBuffer)
	{
		glBindVertexArray(VAO);
		iBuffer->Bind();

		indexBuffer = iBuffer;
	}

	const std::shared_ptr<IndexBuffer>& VertexArray::GetIndexBuffer() const
	{
		return indexBuffer;
	}

	VertexArray* VertexArray::Create()
	{
		return new VertexArray;
	}
}
