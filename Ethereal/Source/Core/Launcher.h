#pragma once
#include <Core/Application.h>

extern Ethereal::Application* Ethereal::CreateApplication();

int main()
{
	auto app = Ethereal::CreateApplication();
	app->Run();
	delete app;

	return 0;
}
