#pragma once

#ifdef ETHEREAL_EXPORTS
	#define ETHEREAL_API __declspec(dllexport)
#else
	#define ETHEREAL_API __declspec(dllimport)
#endif // ETHEREAL_EXPORTS