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
#include <algorithm>

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

		size_t position = output.find("[download] Destination: ");

		std::cout << output;

		if (position != std::string::npos) {
			std::string filename = output.substr(position + strlen("[download] Destination: "));
			//filename.
			return filename;
		}
		else {
			std::cout << "Couldnt find file path!" << std::endl;
		}
		
		return "Couldnt find file path!";
	}

	void addSong(std::string url) {
		std::string filePath = downloadSong(url);

		Song song;
		song.storageLocation = filePath;

		std::cout << song.storageLocation;

		std::string songName;

		std::cout << "Enter the name of the song: ";
		std::cin >> songName;
	
		song.songName = songName;
		
		std::string artist;

		std::cout << "Enter the artist: ";
		std::cin >> artist;
		song.artist = artist;

		if (!addToSongDirectory()) {
			std::cout << "Song coulnt be added to directory, deleting may be required!";
		}
	}

	int addToSongDirectory(Song song) {
		Json::Reader reader;
		Json::StyledWriter writer;
		Json::Value root;

		std::ifstream file("SongDirectory.json");

		if (!reader.parse(file, root, true)) {
			std::cout << "Something went wrong during loading" << "\n" << reader.getFormatedErrorMessages();
			return 0;
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

		return 1;
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

