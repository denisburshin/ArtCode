#include "Layer.h"
#include "Ethereal/Launcher.h"

class Application : public Ethereal::CoreApplication
{
public:
	Application()
	{
		PushApplication(new Layer);
	}

	~Application()
	{

	}
};

Ethereal::CoreApplication* Ethereal::CreateApplication()
{
	return new Application;
}

