#include <EditorLayer.h>
#include <imgui.h>
#include <imgui_stdlib.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <Core/Scene/SceneSerializer.h>
#include <ImGuizmo.h>
#include <Utility/Math.h>

EditorLayer::EditorLayer()
	: backgroundColor(0.0f), controller(1280.0f / 720.0f)
{
	InterfacePreparation();

	Ethereal::Renderer2D::Init();

	scene.reset(new Ethereal::Scene);
#if 0
	Ethereal::Entity square = scene->CreateEntity();

	square.AddComponent<Ethereal::SpriteComponent>(glm::vec4{ 1.0f, 0.0f, 0.0f, 1.0f });
	//square.AddComponent<Ethereal::TagComponent>("Square #1");

	Ethereal::Entity camera = scene->CreateEntity();
	camera.AddComponent<Ethereal::CameraComponent>();
	//camera.AddComponent<Ethereal::TagComponent>("Camera #1");

	texture.reset(Ethereal::Texture2D::Create("Assets/Textures/default.png"));
#endif
	framebuffer = Ethereal::Framebuffer::Create({ 1280, 720 });
	sceneHierarchy.SetContext(scene);
}

void EditorLayer::OnUpdate()
{
	Ethereal::Renderer::ResetStatistic();
	{
		if (Ethereal::FramebufferSpecification spec = framebuffer->GetSpecification();
			viewportSize.x > 0.0f && viewportSize.y > 0.0f &&
			(spec.width != viewportSize.x || spec.height != viewportSize.y))
		{
			framebuffer->Resize((uint32_t)viewportSize.x, (uint32_t)viewportSize.y);
			scene->OnViewPortResize((uint32_t)viewportSize.x, (uint32_t)viewportSize.y);
		}
		// Here we call draw functions
		framebuffer->Use();

		controller.OnUpdate();

		Ethereal::Renderer::SetClearColor(backgroundColor);
		Ethereal::Renderer::ClearColor();

		//Ethereal::Renderer2D::BeginScene(controller.GetCamera());
		
		scene->OnUpdate();
		//glm::mat4 scale = glm::scale(glm::identity<glm::mat4>(), this->scale);

		//scene.texture->Bind();
		//Ethereal::Renderer2D::DrawQuad({ -1.0f, 0.0f }, { 0.8f, 0.8f }, { 0.0f, 0.0f, 1.0f, 1.0f });
		//Ethereal::Renderer2D::DrawQuad({ 0.0f, 0.0f }, { 1.0f, 1.0f }, { 1.0f, 0.0f, 0.0f, 1.0f });
		//Ethereal::Renderer2D::DrawQuad({ 0.0f, 0.0f, -0.1f }, { 10.0f, 10.0f }, texture);
		//Ethereal::Renderer2D::DrawRotatedQuad({ 0.0f, 0.0f }, { 1.0f, 1.0f }, 0.0f, texture);

		framebuffer->Unuse();
	}
}

void EditorLayer::OnGUIRender()
{
	ImGui::DockSpaceOverViewport(ImGui::GetMainViewport());

	if (ImGui::BeginMainMenuBar())
	{
		if (ImGui::BeginMenu("File"))
		{
			if (ImGui::MenuItem("Save"))
			{
				Ethereal::SceneSerializer serializer(scene);
				serializer.Serialize("Assets/Scenes/Scene.eth");
			}
			if (ImGui::MenuItem("Load"))
			{
				Ethereal::SceneSerializer serializer(scene);
				serializer.Deserialize("Assets/Scenes/Cube.eth");
			}
			if (ImGui::MenuItem("Exit"))
				Ethereal::Application::Get()->Close();

			ImGui::EndMenu();
		}
		ImGui::EndMainMenuBar();
	}

	/*if (ImGui::Begin("Properties"))
	{
		ImGui::Text("General");
		ImGui::ColorEdit4("Background Color", glm::value_ptr(backgroundColor));

		//std::string texture_path;
		//ImGui::Text("Model");
		//ImGui::SliderFloat2("Scale", glm::value_ptr(scale), 0.0f, 2.0f);
		//if (ImGui::InputText("Texture", &texture_path, ImGuiInputTextFlags_EnterReturnsTrue))
		//{
		//	scene.texture.reset(Ethereal::Texture2D::Create(texture_path));
		//}

	}
	ImGui::End();*/

	sceneHierarchy.OnGUIRender();

	if (ImGui::Begin("Render Statistic"))
	{
		ImGui::BulletText("Draw Calls: %i", Ethereal::Renderer::DrawCalls());
		ImGui::End();
	}

	if (ImGui::Begin("Viewport"))
	{
		auto panelSize = ImGui::GetContentRegionAvail();
		if (viewportSize != *((glm::vec2*)&panelSize))
		{
			framebuffer->Resize((uint32_t)panelSize.x, (uint32_t)panelSize.y);
			viewportSize = { panelSize.x, panelSize.y };
		}
		auto colorAttachment = framebuffer->GetColorAttachment();
		ImGui::Image((void*)(uint64_t)colorAttachment, ImVec2(viewportSize.x, viewportSize.y), ImVec2(0, 1), ImVec2(1, 0));

		//GIZMO
		Ethereal::Entity selectedEntity = sceneHierarchy.GetSelectedEntity();
		if (selectedEntity && currentGuizmo != -1)
		{
			ImGuizmo::SetOrthographic(false);
			ImGuizmo::SetDrawlist();
			float windowWidth = (float)ImGui::GetWindowWidth();
			float windowHeight = (float)ImGui::GetWindowHeight();
			ImGuizmo::SetRect(ImGui::GetWindowPos().x, ImGui::GetWindowPos().y, windowWidth, windowHeight);

			//CAMERA
			auto ce = scene->GetPrimaryCameraEntity();
			const auto& camera = ce.GetComponent<Ethereal::CameraComponent>().camera;
			const glm::mat4& projection = camera.GetProjection();
			glm::mat4 cv = glm::inverse(ce.GetComponent<Ethereal::TransformComponent>().GetTransform());

			auto& tc = selectedEntity.GetComponent<Ethereal::TransformComponent>();
			glm::mat4 transform = tc.GetTransform();
			ImGuizmo::Manipulate(glm::value_ptr(cv), glm::value_ptr(projection),
				(ImGuizmo::OPERATION)currentGuizmo, ImGuizmo::LOCAL, glm::value_ptr(transform));

			if (ImGuizmo::IsUsing())
			{
				glm::vec3 tranlation, rotation, scale;
				Ethereal::Utility::DecomposeTransform(transform, tranlation, rotation, scale);
				
				glm::vec3 deltaRotation = rotation - tc.Rotation;
				tc.Translation = tranlation;
				tc.Rotation += deltaRotation;
				tc.Scale = scale;
			}
		}

		ImGui::End();
	}
}

void EditorLayer::OnEvent(Ethereal::Event& event)
{
	Ethereal::EventDispatcher dispatcher(event);
	dispatcher.Dispatch<Ethereal::KeyEvent>(std::bind(&EditorLayer::OnKeyEvent, this, std::placeholders::_1));

	controller.OnEvent(event);
}

bool EditorLayer::OnKeyEvent(Ethereal::KeyEvent& event)
{
	if (Ethereal::Input::IsKeyPressed(Key::A))
	{
		currentGuizmo = ImGuizmo::OPERATION::TRANSLATE;
	}
	else if (Ethereal::Input::IsKeyPressed(Key::W))
	{
		currentGuizmo = ImGuizmo::OPERATION::ROTATE;
	}
	else if (Ethereal::Input::IsKeyPressed(Key::S))
	{
		currentGuizmo = ImGuizmo::OPERATION::SCALE;
	}
	return true;
}

std::unique_ptr<Ethereal::Layer> EditorLayer::GetLayer() const
{
	return std::make_unique<EditorLayer>();
}

void EditorLayer::InterfacePreparation() const
{
	ImGui::StyleColorsDark();

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
