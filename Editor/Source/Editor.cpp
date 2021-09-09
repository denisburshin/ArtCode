#include <EditorLayer.h>
#include <Core/Launcher.h>

class Editor final : public Ethereal::Application
{
public:
	Editor()
	{
		PushLayer(new EditorLayer);
	}

	~Editor()
	{

	}
};

Ethereal::Application* Ethereal::CreateApplication()
{
	return new Editor;
}
