#include "FileManagment.h"

	void reloadDirectory(Json::Value& loadedDirectory) {
		Json::Reader reader;

		if (std::filesystem::exists("SongDirectory.json")) {
			std::ifstream file("SongDirectory.json");

			if (!reader.parse(file, loadedDirectory, true)) {
				std::cout << "Something went wrong during loading" << "\n" << reader.getFormatedErrorMessages();
			}

			std::cout << "Reloaded song directory\n";
		}
		else {
			std::ofstream outputFile("SongDirectory.json");
			if (outputFile.is_open()) {
				outputFile << "{}";
				outputFile.close();
			}
			reloadDirectory(loadedDirectory);
		}

	}

	std::string convertToAudio(std::string filePath, SaveStrategy strategy) {

		size_t dotPosition = filePath.find_last_of('.');

		std::string fileNameWithoutExtension;

		if (dotPosition != std::string::npos) {
			fileNameWithoutExtension = filePath.substr(0, dotPosition);
		}

		if (strategy == WAV) {
			if (convertWithFFMPEG(filePath, fileNameWithoutExtension, ".wav") == 0) {
				return fileNameWithoutExtension + ".wav";
			}
			else {
				throw std::runtime_error("Error converting to .wav");
			}
		}
		else if (strategy == MP3) {
			if (convertWithFFMPEG(filePath, fileNameWithoutExtension, ".mp3") == 0) {
				return fileNameWithoutExtension + ".mp3";
			}
			else {
				throw std::runtime_error("Error converting to .mp3");
			}
		}
	}

	int convertWithFFMPEG(std::string fullPath, std::string pathWithoutExtension, std::string extension) {
		int result = system(("ffmpeg -i \"" + fullPath + "\" -vn \"" + pathWithoutExtension + extension + "\"").c_str());

		return result;
	}

	//Song path in this case means the storage location without a file extension e.g:
	//C://Frontend//MusicLibrary//TJ_Beastboy - Godzilla
	void removeSongFiles(std::string songPath) {
		std::cout << "Removing song files..." << std::endl;
		while(true) {}
	}

	int addToSongDirectory(Song song) {
		Json::Reader reader;
		Json::StyledWriter writer;
		Json::Value root;

		std::ifstream file("SongDirectory.json");

		if (!reader.parse(file, root, true)) {
			std::cout << "Something went wrong during loading" << "\n" << reader.getFormatedErrorMessages();
			removeSongFiles(song.storageLocation);
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