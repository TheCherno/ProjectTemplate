#include "Backend.h"
#include "Settings.h"

#include "MusicTypes.h"

#include <fstream>
#include "json\json.h"

#include <iostream>

namespace Backend {
	Json::Value loadedDirectory;

	int downloadSong(std::string url) {
		std::string cmd = "youtube-dl \"" + url + "\" -o " + getDownloadDirectory();
		std::cout << cmd;
		int status = std::system(cmd.c_str());
		return status;
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

	/// <summary>
	/// Test
	/// </summary>
	/// <param id="songName">Test</param>
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

