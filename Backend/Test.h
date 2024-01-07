#pragma once

#ifdef BACKEND_EXPORTS
#define BACKEND __declspec(dllexport)
#else
#define BACKEND __declspec(dllimport)
#endif

extern "C" BACKEND void printTest();