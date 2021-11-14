#include "Renderer2D.h"
#include <Core/Render/Renderer.h>
#include <glm/gtc/matrix_transform.hpp>
#include <array>

namespace Ethereal
{
	struct QuadVertex
	{
		glm::vec3 position;
		glm::vec4 color;
		glm::vec2 texCoord;
		float texIndex;
	};

	struct Storage2D
	{
		const uint32_t MaxQuads = 10000;
		const uint32_t MaxVertices = MaxQuads * 4;
		const uint32_t MaxIndices = MaxQuads * 6;
		static const uint32_t MaxTextureSlots = 32;

		std::shared_ptr<VertexArray> VAO;
		std::shared_ptr<VertexBuffer> VBO;
		std::shared_ptr<Shader> shader;
		std::shared_ptr<Texture2D> whiteTexture;

		uint32_t indexCount = 0;

		QuadVertex* quadVertexBufferBase = nullptr;
		QuadVertex* quadVertexBufferPtr = nullptr;

		std::array<std::shared_ptr<Texture2D>, MaxTextureSlots> TextureSlots;
		uint32_t TextureSlotIndex = 1; // 0 - WhiteTexture;

		glm::vec4 quadVertexPositions[4];
	};

	static Storage2D *storage;

	void Renderer2D::Init()
	{
		storage = new Storage2D;

		storage->VAO.reset(VertexArray::Create());

		//float vertices[] = {
		//	-0.5f, -0.5f, 0.0f, 0.0f, 0.0f,
		//	-0.5f, 0.5f, 0.0f, 0.0f, 1.0f,
		//	0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
		//	0.5f, 0.5f, 0.5f, 1.0f, 1.0f
		//};

		storage->VBO.reset(VertexBuffer::Create(storage->MaxVertices * sizeof(QuadVertex)));

		Ethereal::BufferLayout layout = {
			{ Ethereal::ShaderDataType::Vec3f, "pos" },
			{ Ethereal::ShaderDataType::Vec4f, "color"},
			{ Ethereal::ShaderDataType::Vec2f, "tex" },
			{ Ethereal::ShaderDataType::Float, "texIndex"}
		};

		storage->VBO->SetLayout(layout);
		storage->VAO->AddVertexBuffer(storage->VBO);

		storage->quadVertexBufferBase = new QuadVertex[storage->MaxVertices];

		auto indices = std::make_unique<uint32_t[]>(storage->MaxIndices);

		//unsigned int indices[] = {
		//	0, 1, 2, 2, 3, 1
		//};

		uint32_t offset = 0;
		for (uint64_t i = 0; i < storage->MaxIndices; i += 6)
		{
			indices[i + 0] = offset + 0;
			indices[i + 1] = offset + 1;
			indices[i + 2] = offset + 2;

			indices[i + 3] = offset + 2;
			indices[i + 4] = offset + 3;
			indices[i + 5] = offset + 0;

			offset += 4;
		}

		std::shared_ptr<IndexBuffer> IBO;
		IBO.reset(IndexBuffer::Create(indices.get(), storage->MaxIndices));

		storage->VAO->AddIndexBuffer(IBO);

		storage->whiteTexture.reset(Texture2D::Create(1, 1));
		uint32_t whiteTextureData = 0xffffffff;
		storage->whiteTexture->SetData(&whiteTextureData, sizeof(uint32_t));

		int32_t samplers[storage->MaxTextureSlots];
		for (uint32_t i = 0; i < storage->MaxTextureSlots; ++i)
		{
			samplers[i] = i;
		}

		storage->shader.reset(new Ethereal::Shader("Assets/Shaders/Texture.vert", "Assets/Shaders/Texture.frag"));
		storage->shader->Use();
		storage->shader->SetIntArray("u_texture", samplers, storage->MaxTextureSlots);

		storage->TextureSlots[0] = storage->whiteTexture;

		storage->quadVertexPositions[0] = { -0.5f, -0.5f, 0.0f, 1.0f };
		storage->quadVertexPositions[1] = {  0.5f, -0.5f, 0.0f, 1.0f };
		storage->quadVertexPositions[2] = {  0.5f,  0.5f, 0.0f, 1.0f };
		storage->quadVertexPositions[3] = { -0.5f,  0.5f, 0.0f, 1.0f };
	}

	void Renderer2D::Shutdown()
	{
		delete storage;
	}

	void Renderer2D::BeginScene(const Camera& camera, const glm::mat4& transform)
	{
		glm::mat4 viewProjection = camera.GetProjection() * glm::inverse(transform);

		storage->shader->Use();
		storage->shader->UploadUniformMat4("u_ProjectionView", viewProjection);
		storage->shader->UploadUniformMat4("u_ModelMatrix", glm::mat4(1.0f));

		storage->indexCount = 0;
		storage->quadVertexBufferPtr = storage->quadVertexBufferBase;

		storage->TextureSlotIndex = 1;
	}

	void Renderer2D::BeginScene(const OrthographicCamera& camera)
	{
		storage->shader->Use();
		storage->shader->UploadUniformMat4("u_ProjectionView", camera.GetProjectionViewMatrix());
		storage->shader->UploadUniformMat4("u_ModelMatrix", glm::mat4(1.0f));

		storage->indexCount = 0;
		storage->quadVertexBufferPtr = storage->quadVertexBufferBase;

		storage->TextureSlotIndex = 1;
	}

	void Renderer2D::EndScene()
	{
		uint32_t dataSize = (uint8_t*)storage->quadVertexBufferPtr - (uint8_t*)storage->quadVertexBufferBase;

		storage->VBO->SetData(storage->quadVertexBufferBase, dataSize);

		for (uint32_t i = 0; i < storage->TextureSlotIndex; ++i)
		{
			storage->TextureSlots[i]->Bind(i);
		}

		Renderer::DrawIndexed(storage->VAO, storage->indexCount);
	}

	void Renderer2D::DrawQuad(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color)
	{
		DrawQuad({ position.x, position.y, 0.0f }, size, color);
	}

	void Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color)
	{
		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position)
			* glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });

		DrawQuad(transform, color);

		/*storage->shader->SetVec4("u_color", color);

		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position) * 
			glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });
		
		storage->shader->UploadUniformMat4("u_ModelMatrix", transform);

		storage->VAO->Bind();
		storage->whiteTexture->Bind();
		Renderer::DrawIndexed(storage->VAO);
		storage->whiteTexture->Unbind();*/
	}

	void Renderer2D::DrawQuad(const glm::vec2& position, const glm::vec2& size, std::shared_ptr<Texture2D>& texture)
	{
		DrawQuad({ position.x, position.y, 0.0f }, size, texture);
	}

	void Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec2& size, std::shared_ptr<Texture2D>& texture)
	{
		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position)
			* glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });

		DrawQuad(transform, texture);

		/*storage->shader->SetVec4("u_color", glm::vec4(1.0f));

		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position) *
			glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });

		storage->shader->UploadUniformMat4("u_ModelMatrix", transform);

		storage->VAO->Bind();
		texture->Bind();
		Renderer::DrawIndexed(storage->VAO);
		texture->Unbind();*/
	}

	void Renderer2D::DrawRotatedQuad(const glm::vec2& position, const glm::vec2& size, float rotation, std::shared_ptr<Texture2D>& texture)
	{
		DrawRotatedQuad({ position.x, position.y, 0.0f }, size, rotation, texture);
	}

	void Renderer2D::DrawRotatedQuad(const glm::vec3& position, const glm::vec2& size, float rotation, std::shared_ptr<Texture2D>& texture)
	{
		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position)
			* glm::rotate(glm::mat4(1.0f), glm::radians(rotation), { 0.0f, 0.0f, 1.0f })
			* glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });
		
		DrawQuad(transform, texture);
	}

	void Renderer2D::DrawQuad(const glm::mat4& transform, const glm::vec4& color)
	{
		const size_t quadVertexCount = 4;
		const glm::vec2  textureCoords[] = {
			{0.0f, 0.0f},
			{1.0f, 0.0f},
			{1.0f, 1.0f},
			{0.0f, 1.0f}
		};
		const float textureIndex = 0.0f; // White Texture

		for (auto i = 0; i < quadVertexCount; ++i)
		{
			storage->quadVertexBufferPtr->position = transform * storage->quadVertexPositions[i];
			storage->quadVertexBufferPtr->color = color;
			storage->quadVertexBufferPtr->texCoord = textureCoords[i];
			storage->quadVertexBufferPtr->texIndex = textureIndex;
			++storage->quadVertexBufferPtr;
		}

		storage->indexCount += 6;
	}
	
	void Renderer2D::DrawQuad(const glm::mat4& transform, std::shared_ptr<Texture2D>& texture)
	{
		const size_t quadVertexCount = 4;
		const glm::vec2  textureCoords[] = { 
			{0.0f, 0.0f},
			{1.0f, 0.0f},
			{1.0f, 1.0f},
			{0.0f, 1.0f} 
		};
		const glm::vec4 color = { 1.0f, 1.0f, 1.0f, 1.0f };

		float textureIndex = 0.0f;

		for (uint32_t i = 1; i < storage->TextureSlotIndex; ++i)
		{
			if (*storage->TextureSlots[i].get() == *texture.get())
			{
				textureIndex = (float)i;
				break;
			}
		}

		if (textureIndex == 0.0f)
		{
			textureIndex = (float)storage->TextureSlotIndex;
			storage->TextureSlots[storage->TextureSlotIndex] = texture;
			++storage->TextureSlotIndex;
		}

		for (auto i = 0; i < quadVertexCount; ++i)
		{
			storage->quadVertexBufferPtr->position = transform * storage->quadVertexPositions[i];
			storage->quadVertexBufferPtr->color = color;
			storage->quadVertexBufferPtr->texCoord = textureCoords[i];
			storage->quadVertexBufferPtr->texIndex = textureIndex;
			++storage->quadVertexBufferPtr;
		}
		storage->indexCount += 6;
	}
};