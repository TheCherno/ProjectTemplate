#include "Backend.h"
#include "Settings.h"

#include "MusicTypes.h"

#include <fstream>
#include "json\json.h"

#include <iostream>

namespace Backend {

	int downloadSong(std::string url) {
		std::string cmd = "youtube-dl \"" + url + "\" -o " + getDownloadDirectory();
		std::cout << cmd;
		int status = std::system(cmd.c_str());
		return status;
	}

	int addToSongDirectory(Song song) {
		Json::Reader reader;
		Json::Value root;

		std::ifstream file("MusicLibrary.json");
		file >> root;

		std::string data;

		file.close();

		return 0;
	}
}

