#pragma once

#ifdef _WIN32
	#define VRMLPROCESSING_API __declspec(dllexport)
#else
	#define VRMLPROCESSING_API
#endif