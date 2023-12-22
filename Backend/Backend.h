#pragma once
#include <string>
#include <iostream>

#include "MusicTypes.h"

namespace Backend {
	std::string downloadSong(std::string url);
	int addToSongDirectory(Song song);
	void reloadDirectory();
	std::string convertToWav(std::string filePath);
	void playSong(std::string songName);
	void addSong(std::string url);
	Song getSong(std::string songName);
	void printAndHandleInput();
	void setVolume(int volume);
	void checkFFMPEGInstallation();
}