#pragma once
#include <stdio.h>
#include <string>
#include "AudioManager.h"

#ifdef BACKEND_EXPORTS
#define BACKEND __declspec(dllexport)
#else
#define BACKEND __declspec(dllimport)
#endif

extern "C" BACKEND void printTest();
extern "C" BACKEND void playSong(const char* songName);
extern "C" BACKEND void setVolume(int volume);