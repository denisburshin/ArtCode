#include <Layer.h>
#include <glm/gtc/matrix_transform.hpp>

Layer::Layer()
	: camera(-1.6f, 1.6f, -0.9f, 0.9f), transform(0.0f)
{
	shader.reset(new Ethereal::Shader("Shaders/Default.vert", "Shaders/Default.frag"));

	VAO.reset(Ethereal::VertexArray::Create());

	float vertices[] = {
		-0.5f, -0.5f, 0.0f,
		-0.5f, 0.5f, 0.0f,
		0.5f, -0.5f, 0.0f,
		0.5f, 0.5f, 0.5f
	};

	VBO.reset(Ethereal::VertexBuffer::Create(vertices, sizeof(vertices)));

	Ethereal::BufferLayout layout = {
		{ Ethereal::ShaderDataType::Vec3f, "pos" }
	};

	VBO->SetLayout(layout);
	VAO->AddVertexBuffer(VBO);

	unsigned int indices[] = {
		0, 1, 2, 2, 3, 1
	};

	IBO.reset(Ethereal::IndexBuffer::Create(indices, sizeof(indices)));
	VAO->AddIndexBuffer(IBO);
}

void Layer::OnUpdate()
{
	Ethereal::Renderer::SetClearColor({ 0.0f, 0.0f, 0.0f, 1.0f });
	Ethereal::Renderer::ClearColor();
	Ethereal::Renderer::BeginScene(camera);

	glm::mat4 scale = glm::scale(glm::identity<glm::mat4>(), glm::vec3(0.2f, 0.2f, 1.0f));

	Ethereal::Renderer::Submit(VAO, shader, glm::translate(glm::identity<glm::mat4>(), transform) * scale);
	Ethereal::Renderer::EndScene();
}

void Layer::OnEvent(Ethereal::Event& event)
{
	Ethereal::EventDispatcher dispatcher(event);
	dispatcher.Dispatch<Ethereal::KeyEvent>(std::bind(&Layer::OnKeyEvent, this, std::placeholders::_1));
}

bool Layer::OnKeyEvent(Ethereal::KeyEvent& event)
{
	int key = event.GetKey();
	if (key == KEY_W)
		transform.y += 0.05f;
	else if (key == KEY_S)
		transform.y -= 0.05f;
	else if (key == KEY_A)
		transform.x -= 0.05f;
	else if (key == KEY_D)
		transform.x += 0.05f;

	return false;
}