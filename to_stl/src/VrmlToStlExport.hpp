#pragma once

#ifdef _WIN32
#define VRMLTOSTL_API __declspec(dllexport)
#else
#define VRMLTOSTL_API
#endif