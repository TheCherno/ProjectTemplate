#pragma once
#include <iostream>
#include <string>

#include <SFML/Audio.hpp>
#include "MusicTypes.h"

namespace Backend {
	std::string downloadSong(std::string url);
	void playSong(std::string songName);
	void addSong(std::string url, std::string songName, std::string artist);
	Song getSong(std::string songName);
	void printAndHandleInput(sf::Sound& sound, sf::SoundBuffer& buffer);
	void setVolume(int volume, sf::Sound& sound);
	void checkFFMPEGInstallation();
	Playlist getPlaylist(std::string playlistName);
	Song getNextSongFromPlaylist();

	void awaitEnter();

	void initializeApplication(bool debug);
}