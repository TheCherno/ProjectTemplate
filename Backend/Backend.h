#pragma once
#include <string>
#include <iostream>

#include "MusicTypes.h"

namespace Backend {
	std::string downloadSong(std::string url);
	void addSong(std::string url);
	Song getSong(std::string songName);
}