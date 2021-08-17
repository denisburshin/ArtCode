#include "Application.h"

Ethereal::CoreApplication* Ethereal::CreateApplication()
{
	return new Application;
}

Application::Application()
	: CoreApplication(), camera(-1.0f, 1.0f, -1.0f, 1.0f)
{
	Ethereal::Shader v("Default.vert", Ethereal::Type::Vertex);
	Ethereal::Shader f("Default.frag", Ethereal::Type::Fragment);

	program.reset(new Ethereal::Program(std::vector<Ethereal::Shader>({ v, f })));

	VAO.reset(Ethereal::VertexArray::Create());

	float vertices[3 * 3] = {
		-0.5f, -0.5f, 0.0f,
		0.5f, -0.5f, 0.0f,
		0.0f, 0.5f, 0.0f
	};

	VBO.reset(Ethereal::VertexBuffer::Create(vertices, sizeof(vertices)));

	Ethereal::BufferLayout layout = {
		{ Ethereal::ShaderDataType::Vec3f, "pos" }
	};

	VBO->SetLayout(layout);
	VAO->AddVertexBuffer(VBO);

	unsigned int indices[3] = {
		0, 1, 2
	};

	IBO.reset(Ethereal::IndexBuffer::Create(indices, sizeof(indices)));
	VAO->AddIndexBuffer(IBO);
}

void Application::OnUpdate()
{
	Ethereal::Renderer::SetClearColor({ 0.0f, 0.0f, 0.0f, 1.0f });
	Ethereal::Renderer::ClearColor();
	Ethereal::Renderer::BeginScene(camera);
	Ethereal::Renderer::Submit(VAO, program);
	Ethereal::Renderer::EndScene();
}
