#pragma once
#include <string>
#include <iostream>

#include "MusicTypes.h"

namespace Backend {
	int downloadSong(std::string url);
	int addToSongDirectory(Song song);
	Song getSong(std::string songName);
}