#pragma once
#include <string>
#include <iostream>

namespace Backend {
	int downloadSong(std::string url);
	int addToSongDirectory(Song song);
}