#include <Scene.h>
#include <Core/Launcher.h>

class ClientApplication : public Ethereal::Application
{
public:
	ClientApplication()
	{
		PushLayer(new Scene);
	}

	~ClientApplication()
	{

	}
};

Ethereal::Application* Ethereal::CreateApplication()
{
	return new ClientApplication;
}

