#include <Ethereal/GUI/GUI.h>
#include <Platform/OpenGL/GUIRenderer.h>
#include <GLFW/glfw3.h>

namespace Ethereal
{
	GUI::GUI()
	{
		ImGui::CreateContext();
		ImGui::StyleColorsDark();

		ImGuiIO& io = ImGui::GetIO();

		io.BackendFlags |= ImGuiBackendFlags_HasMouseCursors;
		io.BackendFlags |= ImGuiBackendFlags_HasSetMousePos;

		ImGui_ImplOpenGL3_Init("#version 460");
	}

	GUI::~GUI()
	{

	}

	void GUI::OnUpdate()
	{

		ImGuiIO& io = ImGui::GetIO();

		io.DisplaySize = ImVec2(1280, 720);

		ImGui_ImplOpenGL3_NewFrame();
		ImGui::NewFrame();

		static bool show = true;

		ImGui::ShowDemoWindow(&show);

		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

	}

	void GUI::OnEvent(Event& event)
	{

	}

	bool GUI::OnKeyEvent(Ethereal::KeyEvent& event)
	{
		return true;
	}

};
