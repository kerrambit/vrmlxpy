#pragma once

#ifdef _WIN32
#define VRMLTOGEOM_API __declspec(dllexport)
#else
#define VRMLTOGEOM_API
#endif