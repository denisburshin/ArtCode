#include <Core/GUI/GUI.h>

#include <imgui.h>
#include <backends/imgui_impl_glfw.h>
#include <backends/imgui_impl_opengl3.h>
#include <Core/Application.h>

namespace Ethereal
{
	GUI::GUI()
	{
		ImGui::CreateContext();

		auto app = Application::Get();
		auto window = static_cast<GLFWwindow*>(app->GetWindow().GetNativeWindow());

		ImGui_ImplGlfw_InitForOpenGL(window, true);
		ImGui_ImplOpenGL3_Init("#version 460");
	}

	GUI::~GUI()
	{
		ImGui_ImplOpenGL3_Shutdown();
		ImGui_ImplGlfw_Shutdown();
		ImGui::DestroyContext();
	}

	void GUI::OnEvent(Event& event) const
	{
		ImGuiIO& io = ImGui::GetIO();
		event.handled = io.WantCaptureMouse;
		event.handled = io.WantCaptureKeyboard;
	}

	void GUI::Begin()
	{
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();

		ImGui::NewFrame();
	}

	void GUI::End()
	{
		ImGuiIO& io = ImGui::GetIO();
		io.DisplaySize = ImVec2(1280.0f, 720.0f);

		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
	}
};
