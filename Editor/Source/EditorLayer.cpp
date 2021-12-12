#include <EditorLayer.h>
#include <imgui.h>
#include <imgui_stdlib.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <Core/Scene/SceneSerializer.h>
#include <ImGuizmo.h>
#include <Utility/Math.h>
#include <Utility/FileDialog.h>

EditorLayer::EditorLayer()
	: backgroundColor(0.0f), controller(1280.0f / 720.0f)
{
	InterfacePreparation();

	Ethereal::Renderer2D::Init();

	scene.reset(new Ethereal::Scene);

	editorCamera = Ethereal::EditorCamera(45.0f, 1.778f, 0.1f, 1000.0f);
#if 0
	Ethereal::Entity square = scene->CreateEntity();

	square.AddComponent<Ethereal::SpriteComponent>(glm::vec4{ 1.0f, 0.0f, 0.0f, 1.0f });
	//square.AddComponent<Ethereal::TagComponent>("Square #1");

	Ethereal::Entity camera = scene->CreateEntity();
	camera.AddComponent<Ethereal::CameraComponent>();
	//camera.AddComponent<Ethereal::TagComponent>("Camera #1");

	texture.reset(Ethereal::Texture2D::Create("Assets/Textures/default.png"));
#endif

	Ethereal::FramebufferSpecification fbSpec;

	fbSpec.Attachments = { 
		Ethereal::FramebufferTextureFormat::RGBA8,
		Ethereal::FramebufferTextureFormat::RED_INTEGER
	};

	fbSpec.width = 1280;
	fbSpec.height = 720;

	framebuffer = Ethereal::Framebuffer::Create(fbSpec);
	sceneHierarchy.SetContext(scene);

	currentGuizmo = ImGuizmo::OPERATION::TRANSLATE;
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
			editorCamera.SetViewportSize(viewportSize.x, viewportSize.y);
			scene->OnViewPortResize((uint32_t)viewportSize.x, (uint32_t)viewportSize.y);
		}
		// Here we call draw functions
		framebuffer->Use();

		controller.OnUpdate();
		editorCamera.OnUpdate();
		Ethereal::Renderer::SetClearColor(backgroundColor);
		Ethereal::Renderer::ClearColor();

		//Clear entity ID attachment to -1 
		framebuffer->ClearColorAttachment(1, -1);

		//Ethereal::Renderer2D::BeginScene(controller.GetCamera());
		
		scene->OnUpdateEditor(editorCamera);

		auto [mx, my] = ImGui::GetMousePos();
		mx -= viewportBounds[0].x;
		my -= viewportBounds[0].y;
		glm::vec2 viewportSize = viewportBounds[1] - viewportBounds[0];

		my = viewportSize.y - my;

		int mouseX = (int)mx;
		int mouseY = (int)my;

		if (mouseX >= 0 && mouseY >= 0 && mouseX < (int)viewportSize.x && mouseY < (int)viewportSize.y)
		{
			auto data = framebuffer->ReadPixel(1, mouseX, mouseY);
			hoveredEntity = data == -1 ? Ethereal::Entity() : Ethereal::Entity((entt::entity)data, scene.get());
		}

		//scene->OnUpdate();
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
			if (ImGui::MenuItem("New"))
			{
				scene.reset(new Ethereal::Scene);
				scene->OnViewPortResize((uint32_t)viewportSize.x, (uint32_t)viewportSize.y);
				sceneHierarchy.SetContext(scene);
			}

			if (ImGui::MenuItem("Open..."))
			{
				auto filepath = Ethereal::FileDialog::OpenFile("Ethereal Scene (*.eth)\0*.eth\0");
				if (!filepath.empty())
				{
					scene.reset(new Ethereal::Scene);
					scene->OnViewPortResize((uint32_t)viewportSize.x, (uint32_t)viewportSize.y);
					sceneHierarchy.SetContext(scene);

					Ethereal::SceneSerializer serializer(scene);
					serializer.Deserialize(filepath);
				}
			}
			if (ImGui::MenuItem("Save As..."))
			{
				auto filepath = Ethereal::FileDialog::SaveFile("Ethereal Scene (*.eth)\0*.eth\0");
				if (!filepath.empty())
				{
					Ethereal::SceneSerializer serializer(scene);
					serializer.Serialize(filepath);
				}
			}
			if (ImGui::MenuItem("Exit"))
				Ethereal::Application::Get()->Close();

			ImGui::EndMenu();
		}
		ImGui::EndMainMenuBar();
	}

	if (ImGui::Begin("Scene Properties"))
	{
		ImGui::Text("Common");
		ImGui::Checkbox("Grid", &enableGrid);
		ImGui::ColorEdit4("Background", glm::value_ptr(backgroundColor));
	}
	ImGui::End();

	sceneHierarchy.OnGUIRender();

	ImGui::Begin("Viewport");

	viewportHovered = ImGui::IsWindowHovered();

	auto viewportOffset = ImGui::GetCursorPos();

	auto panelSize = ImGui::GetContentRegionAvail();
	viewportSize = {panelSize.x, panelSize.y};

	auto colorAttachment = framebuffer->GetColorAttachment();
	ImGui::Image((void*)(uint64_t)colorAttachment, ImVec2(viewportSize.x, viewportSize.y), ImVec2(0, 1), ImVec2(1, 0));

	auto windowSize = ImGui::GetWindowSize();
	ImVec2 minBound = ImGui::GetWindowPos();
	minBound.x += viewportOffset.x;
	minBound.y += viewportOffset.y;

	ImVec2 maxBound = { minBound.x + windowSize.x, minBound.y + windowSize.y };
	viewportBounds[0] = { minBound.x, minBound.y };
	viewportBounds[1] = { maxBound.x, maxBound.y };

	//GIZMO
	Ethereal::Entity selectedEntity = sceneHierarchy.GetSelectedEntity();
	if (selectedEntity && currentGuizmo != -1)
	{
		ImGuizmo::SetOrthographic(true);
		ImGuizmo::SetDrawlist();
		float windowWidth = (float)ImGui::GetWindowWidth();
		float windowHeight = (float)ImGui::GetWindowHeight();
		ImGuizmo::SetRect(ImGui::GetWindowPos().x, ImGui::GetWindowPos().y, windowWidth, windowHeight);

		//CAMERA
		//ENTITY CAMERA
		//auto ce = scene->GetPrimaryCameraEntity();
		//const auto& camera = ce.GetComponent<Ethereal::CameraComponent>().camera;
		//const glm::mat4& projection = camera.GetProjection();
		//glm::mat4 cv = glm::inverse(ce.GetComponent<Ethereal::TransformComponent>().GetTransform());

		const glm::mat4& projection = editorCamera.GetProjection();
		glm::mat4 cv = editorCamera.GetViewMatrix();

		//ENTITY TRANSFORM
		auto& tc = selectedEntity.GetComponent<Ethereal::TransformComponent>();
		glm::mat4 transform = tc.GetTransform();
		ImGuizmo::Manipulate(glm::value_ptr(cv), glm::value_ptr(projection),
			(ImGuizmo::OPERATION)currentGuizmo, ImGuizmo::LOCAL, glm::value_ptr(transform));

		if (enableGrid)
			ImGuizmo::DrawGrid(&cv[0][0], &projection[0][0], &glm::mat4(1.0f)[0][0], 10.0f);

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

void EditorLayer::OnEvent(Ethereal::Event& event)
{
	controller.OnEvent(event);
	editorCamera.OnEvent(event);
	
	Ethereal::EventDispatcher dispatcher(event);
	dispatcher.Dispatch<Ethereal::KeyEvent>(std::bind(&EditorLayer::OnKeyEvent, this, std::placeholders::_1));
	dispatcher.Dispatch<Ethereal::MousePressedEvent>(std::bind(&EditorLayer::OnMouseButtonPressed, this, std::placeholders::_1));

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

bool EditorLayer::OnMouseButtonPressed(Ethereal::MousePressedEvent& event)
{
	if (event.GetButton() == static_cast<int>(Mouse::LeftClick))
	{
		if (viewportHovered && !ImGuizmo::IsOver() && !Ethereal::Input::IsKeyPressed(Key::LeftCtrl))
			sceneHierarchy.SetSelectedEntity(hoveredEntity);
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
