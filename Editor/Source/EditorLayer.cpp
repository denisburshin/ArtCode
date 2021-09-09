#include <EditorLayer.h>
#include <imgui.h>

EditorLayer::EditorLayer()
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

void EditorLayer::OnUpdate()
{
	Ethereal::Renderer::ResetStatistic();
	{
		// Here we call draw functions
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

	if (ImGui::Begin("Render Statistic"))
	{
		ImGui::BulletText("Draw Calls: %i", Ethereal::Renderer::DrawCalls());
	}
	ImGui::End();
}

void EditorLayer::OnEvent(Ethereal::Event& event)
{

}

bool EditorLayer::OnKeyEvent(Ethereal::KeyEvent& event)
{
	return true;
}

std::unique_ptr<Ethereal::Layer> EditorLayer::GetLayer() const
{
	return std::make_unique<EditorLayer>();
}
