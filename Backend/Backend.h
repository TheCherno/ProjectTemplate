#pragma once
#include <iostream>
#include <string>

#include "MusicTypes.h"

namespace Backend {
	std::string downloadSong(std::string url);
	void playSong(std::string songName);
	void addSong(std::string url);
	Song getSong(std::string songName);
	void printAndHandleInput();
	void setVolume(int volume);
	void checkFFMPEGInstallation();
	Playlist getPlaylist(std::string playlistName);
	Song getNextSongFromPlaylist();

	void awaitEnter();

	void initializeApplication(bool debug);
}