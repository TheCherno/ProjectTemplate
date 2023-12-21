#include "Backend.h"
#include "Settings.h"

#include "MusicTypes.h"

#include "json\json.h"
#include <fstream>

#include <algorithm>
#include <cstdio>
#include <iostream>
#include <memory>
#include <stdexcept>
#include <string>

#include <Windows.h>
#include <mmsystem.h>

#include <SFML/Audio.hpp>

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

		size_t positionDownloaded = output.find("has already been downloaded");

		if (positionDownloaded == std::string::npos) {

			size_t position = output.find("[download] Destination: ");

			std::cout << output;

			if (position != std::string::npos) {
				std::string filename = output.substr(position + strlen("[download] Destination: "));

				size_t pos = filename.find(".webm");

				// Check if ".webm" is found
				if (pos != std::string::npos) {
					// Extract the substring up to the position of ".webm"
					std::string result = filename.substr(0, pos + 5); // +5 to include ".webm"
					//std::cout << "Modified file path: " << result << std::endl;
					return result;
				}
				else {
					std::cout << ".webm not found in the file path." << std::endl;
					return "Wrong file format!";
				}
			}
			else {
				std::cout << "Couldnt find file path!" << std::endl;
			}
		}
		else {
			return "-1";
		}
	}

	void addSong(std::string url) {
		std::string filePath = downloadSong(url);

		if (filePath == "-1") {
			std::cout << "Song already downloaded! Aborting...\n";
			return;
		}

		std::string storageLocation = convertToWav(filePath);

		Song song;
		song.storageLocation = storageLocation;

		std::cout << song.storageLocation;

		std::string songName;

		//std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

		std::cout << "\nEnter the name of the song: \n";
		
		std::getline(std::cin, songName);
	
		song.songName = songName;
		
		std::string artist;

		std::cout << "Enter the artist: \n";
		std::getline(std::cin, artist);
		song.artist = artist;

		try {
			std::uintmax_t fileSize = std::filesystem::file_size(filePath);
			//std::cout << fileSize;
			song.sizeInBytes = fileSize;
		}
		catch (const std::filesystem::filesystem_error& e) {
			std::cerr << "Error: " << e.what() << std::endl;
		}


		if (!addToSongDirectory(song)) {
			std::cout << "Song coulnt be added to directory, deleting may be required!\n";
		}

		reloadDirectory();
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

		//std::cout << root;

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
		else { std::cout << "Song already exists!\n"; }

		return 1;
	}

	std::string convertToWav(std::string filePath) {

		size_t dotPosition = filePath.find_last_of('.');

		std::string fileNameWithoutExtension;

		if (dotPosition != std::string::npos) {
			fileNameWithoutExtension = filePath.substr(0, dotPosition);
		}

		system(("ffmpeg -i \"" + filePath + "\" -vn \"" + fileNameWithoutExtension + ".wav\"").c_str());

		return fileNameWithoutExtension + ".wav";
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
			std::cout << "Couldn't find song in loaded dictionary\n";
			return song;
		}

		return song;
	}

	void reloadDirectory() {
		Json::Reader reader;

		std::ifstream file("SongDirectory.json");

		if (!reader.parse(file, loadedDirectory, true)) {
			std::cout << "Something went wrong during loading" << "\n" << reader.getFormatedErrorMessages();
		}

		std::cout << "Reloaded song directory\n";
	}

	void playSong(std::string songName) {
		//const wchar_t* filePath = L"D:\CPP\Stealify\Frontend\MusicLibrary\TJ_beastboy & Mary Man - Modus.wav";

		sf::SoundBuffer buffer;
		sf::Sound sound;

		std::string path = getSong(songName).storageLocation;

		if (!buffer.loadFromFile(path)) {
			std::cerr << "Failed to load audio file." << std::endl;
		}

		sound.sf::Sound::setBuffer(buffer);

		sound.setVolume(10);

		sound.play();

		while (sound.getStatus() == sf::Sound::Playing) {
			// Update or perform other tasks while the audio is playing
		}
	}
}

