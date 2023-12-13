#pragma once
#include <vector>
#include <string>

struct Song {
	//Point to mp3 or other type of file (.wav, .ogg)
	std::string storageLocation;
	int sizeInBytes;

	std::vector<int> includedPlaylists;

	std::string songName;
	std::string artist;

	bool partiallyLoaded = false;
	bool isPlaying = false;
	bool isPaused = false;

	//TODO
	bool hasLyricsAvailable = false;

	int timeRemaining;
};

struct Playlist {
	std::vector<Song> songs;
	
	std::string name;
	std::string creationDate;

	int songIndex = 0;

	bool isPlaying = false;
	bool isPaused = false;
};
