#include <Layer.h>
#include <imgui.h>
#include <misc/cpp/imgui_stdlib.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

Layer::Layer()
	: camera(-1.6f, 1.6f, -0.9f, 0.9f), transform(0.0f), squareColor(1.0f), bgColor(0.0f), scale(1.0f)
{
	VAO.reset(Ethereal::VertexArray::Create());

	float vertices[] = {
		-0.5f, -0.5f, 0.0f, 0.0f, 0.0f,
		-0.5f, 0.5f, 0.0f, 0.0f, 1.0f,
		0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
		0.5f, 0.5f, 0.5f, 1.0f, 1.0f
	};

	VBO.reset(Ethereal::VertexBuffer::Create(vertices, sizeof(vertices)));

	Ethereal::BufferLayout layout = {
		{ Ethereal::ShaderDataType::Vec3f, "pos" },
		{ Ethereal::ShaderDataType::Vec2f, "tex" }
	};

	VBO->SetLayout(layout);
	VAO->AddVertexBuffer(VBO);

	unsigned int indices[] = {
		0, 1, 2, 2, 3, 1
	};

	IBO.reset(Ethereal::IndexBuffer::Create(indices, sizeof(indices)));
	VAO->AddIndexBuffer(IBO);

	texture = Ethereal::Texture2D::Create("Assets/Textures/silver_glitter.png");

	shader.reset(new Ethereal::Shader("Assets/Shaders/Texture.vert", "Assets/Shaders/Texture.frag"));
	shader->Use();
	shader->UploadUniformInt("u_texture", 0);
}

void Layer::OnUpdate()
{
	Ethereal::Renderer::SetClearColor(bgColor);
	Ethereal::Renderer::ClearColor();
	Ethereal::Renderer::BeginScene(camera);

	//shader->Use();
	//shader->UploadUniformVec3("u_Color", squareColor);

	glm::mat4 scale = glm::scale(glm::identity<glm::mat4>(), this->scale);

	texture->Bind();
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

void Layer::OnGUIRender()
{
	ImGui::Begin("Settings");
	ImGui::Text("General");
	ImGui::ColorEdit3("Background Color", glm::value_ptr(bgColor));

	ImGui::Text("Model");
	ImGui::ColorEdit3("Color", glm::value_ptr(squareColor));
	ImGui::DragFloat2("Scale", glm::value_ptr(scale));

	if (ImGui::InputText("Texture", &tex_path, ImGuiInputTextFlags_EnterReturnsTrue))
	{
		texture.release();
		texture = Ethereal::Texture2D::Create(tex_path);
		shader->Use();
		shader->UploadUniformInt("u_texture", 0);
	}

	ImGui::End();
}
