#pragma once
#include "CoreApplication.h"

extern Ethereal::CoreApplication* Ethereal::CreateApplication();

int main()
{
	auto app = Ethereal::CreateApplication();
	app->Run();
	delete app;

	return 0;
}


