#include <EditorLayer.h>
#include <imgui.h>
#include <imgui_stdlib.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

EditorLayer::EditorLayer()
	: backgroundColor(1.0f), camera(-1.6f, 1.6f, -0.9f, 0.9f), transform(0.0f), scale(1.0f)
{
	InterfacePreparation();

	scene.vao.reset(Ethereal::VertexArray::Create());
	
	Quad quad;
	scene.vertices.insert(scene.vertices.end(), quad.vertices.begin(), quad.vertices.end());

	scene.vbo.reset(Ethereal::VertexBuffer::Create(scene.vertices));

	Ethereal::BufferLayout layout = {
		{ Ethereal::ShaderDataType::Vec3f, "pos" },
		{ Ethereal::ShaderDataType::Vec2f, "tex" }
	};

	scene.vbo->SetLayout(layout);
	scene.vao->AddVertexBuffer(scene.vbo);

	scene.ibo.reset(Ethereal::IndexBuffer::Create(quad.indices.data(), quad.indices.size()));
	scene.vao->AddIndexBuffer(scene.ibo);

	scene.texture.reset(Ethereal::Texture2D::Create("Assets/Textures/default.png"));

	shader.reset(new Ethereal::Shader("Assets/Shaders/Texture.vert", "Assets/Shaders/Texture.frag"));
	shader->Use();
	shader->UploadUniformInt("u_texture", 0);

	framebuffer = Ethereal::Framebuffer::Create({ 1280, 720 });
}

void EditorLayer::OnUpdate()
{
	Ethereal::Renderer::ResetStatistic();
	{
		// Here we call draw functions
		framebuffer->Use();

		Ethereal::Renderer::SetClearColor(backgroundColor);
		Ethereal::Renderer::ClearColor();

		Ethereal::Renderer::BeginScene(camera);

		glm::mat4 scale = glm::scale(glm::identity<glm::mat4>(), this->scale);

		scene.texture->Bind();
		Ethereal::Renderer::Submit(scene.vao, shader, glm::translate(glm::identity<glm::mat4>(), transform) * scale);
		Ethereal::Renderer::EndScene();

		framebuffer->Unuse();
	}
}

void EditorLayer::OnGUIRender()
{
	ImGui::DockSpaceOverViewport(ImGui::GetMainViewport());

	if (ImGui::BeginMainMenuBar())
	{
		if (ImGui::BeginMenu("Options"))
		{
			if (ImGui::MenuItem("Exit"))
				Ethereal::Application::Get()->Close();

			ImGui::EndMenu();
		}
		ImGui::EndMainMenuBar();
	}

	if (ImGui::Begin("Properties"))
	{
		ImGui::Text("General");
		ImGui::ColorEdit4("Background Color", glm::value_ptr(backgroundColor));

		std::string texture_path;
		ImGui::Text("Model");
		ImGui::SliderFloat2("Scale", glm::value_ptr(scale), 0.0f, 2.0f);
		if (ImGui::InputText("Texture", &texture_path, ImGuiInputTextFlags_EnterReturnsTrue))
		{
			scene.texture.reset(Ethereal::Texture2D::Create(texture_path));
		}

	}
	ImGui::End();

	if (ImGui::Begin("Render Statistic"))
	{
		ImGui::BulletText("Draw Calls: %i", Ethereal::Renderer::DrawCalls());
	}
	ImGui::End();

	if (ImGui::Begin("Viewport"))
	{
		auto panelSize = ImGui::GetContentRegionAvail();
		framebuffer->Resize(panelSize.x, panelSize.y);
		auto colorAttachment = framebuffer->GetColorAttachment();
		ImGui::Image((void*)colorAttachment, ImVec2(panelSize.x, panelSize.y));
	}
	ImGui::End();
}

void EditorLayer::OnEvent(Ethereal::Event& event)
{
	Ethereal::EventDispatcher dispatcher(event);
	dispatcher.Dispatch<Ethereal::KeyEvent>(std::bind(&EditorLayer::OnKeyEvent, this, std::placeholders::_1));
}

bool EditorLayer::OnKeyEvent(Ethereal::KeyEvent& event)
{
	int key = event.GetKey();
	if (key == KEY_W)
		transform.y -= 0.05f;
	else if (key == KEY_S)
		transform.y += 0.05f;
	else if (key == KEY_A)
		transform.x -= 0.05f;
	else if (key == KEY_D)
		transform.x += 0.05f;

	return true;
}

std::unique_ptr<Ethereal::Layer> EditorLayer::GetLayer() const
{
	return std::make_unique<EditorLayer>();
}

void EditorLayer::InterfacePreparation() const
{
	ImGui::StyleColorsLight();

	ImGuiStyle& style = ImGui::GetStyle();

	style.WindowRounding = 6.0f;
	style.FrameRounding = 6.0f;
	style.ChildRounding = 6.0f;
	style.PopupRounding = 6.0f;

	ImGuiIO& io = ImGui::GetIO();

	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;

	io.BackendFlags |= ImGuiBackendFlags_HasMouseCursors;
	io.BackendFlags |= ImGuiBackendFlags_HasSetMousePos;
}
