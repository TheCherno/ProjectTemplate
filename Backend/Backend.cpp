#include "Backend.h"
#include "Settings.h"

#include "MusicTypes.h"

#include <fstream>
#include "json\json.h"

#include <iostream>
#include <cstdio>
#include <memory>
#include <string>
#include <stdexcept>

namespace Backend {
	Json::Value loadedDirectory;

	std::string exec(const char* cmd) {
		char buffer[1024];
		std::string result = "";
		FILE* pipe = _popen(cmd, "r");
		if (!pipe) throw std::runtime_error("popen() failed!");

		while (fgets(buffer, sizeof(buffer), pipe) != NULL) {
			result += buffer;
		}

		_pclose(pipe);
		return result;
	}

	//Returns song name
	std::string downloadSong(std::string url) {
		std::string cmd = "youtube-dl \"" + url + "\" -o " + getDownloadDirectory() + " --write-thumbnail --format 251";

		std::string output = exec(cmd.c_str());
		//std::cout << output;
		size_t position = output.find("[download] Destination: ");

		if (position != std::string::npos) {
			std::string filename = output.substr(position + strlen("[download] Destination: "));
			std::cout << "Downloaded file: " << filename << std::endl;
		}
		else {
			std::cout << "File name not found in the output." << std::endl;
		}

		return cmd;
	}

	int addToSongDirectory(Song song) {
		Json::Reader reader;
		Json::StyledWriter writer;
		Json::Value root;

		std::ifstream file("SongDirectory.json");

		if (!reader.parse(file, root, true)) {
			std::cout << "Something went wrong during loading" << "\n" << reader.getFormatedErrorMessages();
		}

		file.close();

		std::cout << root;

		//Json::Value songs = root["songs"];

		if (root["songs"][song.songName].isNull()) {

			root["songs"][song.songName]["songName"] = song.songName;
			root["songs"][song.songName]["storageLocation"] = song.storageLocation;
			root["songs"][song.songName]["artist"] = song.artist;
			root["songs"][song.songName]["sizeInBytes"] = song.sizeInBytes;
			root["songs"][song.songName]["hasLyricsAvailable"] = song.hasLyricsAvailable;

			std::ofstream newFile("SongDirectory.json");

			newFile << writer.write(root);

			newFile.close();
		}
		else { std::cout << "Song already exists!"; }

		return 0;
	}

	int convertToWav(Song song) {
		return 0;
	}

	Song getSong(std::string songName) {
		Song song;

		if (!loadedDirectory["songs"][songName].isNull()) {
			song.songName = loadedDirectory["songs"][songName]["songName"].asString();
			song.storageLocation = loadedDirectory["songs"][songName]["storageLocation"].asString();
			song.artist = loadedDirectory["songs"][songName]["artist"].asString();
			song.sizeInBytes = loadedDirectory["songs"][songName]["sizeInBytes"].asInt();
			song.hasLyricsAvailable = loadedDirectory["songs"][songName]["hasLyricsAvailable"].asBool();
		}
		else {
			std::cout << "Couldn't find song in loaded dictionary";
			return song;
		}

		return song;
	}
}

