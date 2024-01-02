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

	std::string convertToAudio(std::string filePath, SaveStrategy strategy, std::string songName) {

		size_t dotPosition = filePath.find_last_of('.');

		std::string fileNameWithoutExtension;

		if (dotPosition != std::string::npos) {
			fileNameWithoutExtension = filePath.substr(0, dotPosition);
			fileNameWithoutExtension = sanitizeFileName(fileNameWithoutExtension);
		}

		if (strategy == WAV) {
			if (convertWithFFMPEG(filePath, "MusicLibrary/" + songName, ".wav") == 0) {
				return "./MusicLibrary/" + songName + ".wav";
			}
			else {
				throw std::runtime_error("Error converting to .wav");
			}
		}
		else if (strategy == MP3) {
			if (convertWithFFMPEG(filePath, "MusicLibrary/" + songName, ".mp3") == 0) {
				return "./MusicLibrary/" + songName + ".mp3";
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

	//Warning:
	//This method is only for removing files after a download abortion due to an error
	//because it requires the current SaveStrategy it cant be used to delete already saved songs
	void removeSongFiles(std::string songPath, SaveStrategy strategy) {
		std::cout << "Removing song files..." << std::endl;

		std::string path;
		size_t dotPosition = songPath.find_last_of(".");

		if (dotPosition != std::string::npos && dotPosition != 0) {
			path = songPath.substr(0,dotPosition);
		}

		std::string webmFile = "./MusicLibrary/" + path + ".webm";
		std::string jpgFile = "./MusicLibrary/" + path + ".jpg";
		std::string webpFile = "./MusicLibrary/" + path + ".webp";
		std::string audioFile = "./MusicLibrary/" + path + (strategy == MP3 ? ".mp3" : ".wav");

		std::cout << webmFile;

		std::remove(webmFile.c_str());
		std::remove(jpgFile.c_str());
		std::remove(webpFile.c_str());
		std::remove(audioFile.c_str());
	}

	std::string sanitizeFileName(const std::string& input) {
		std::string sanitized;
		for (char c : input) {
			if (std::isalnum(c) || c == ' ' || c == '-' || c == '_' || c == '\\' || c == '/') {
				sanitized += c;
			}
			else if (c == '.') {
				// Allow dots in the filename (extension separator)
				sanitized += c;
			}
		}
		return sanitized;
	}


	int addToSongDirectory(Song song, SaveStrategy strategy) {
		Json::Reader reader;
		Json::StyledWriter writer;
		Json::Value root;

		std::ifstream file("SongDirectory.json");

		if (!reader.parse(file, root, true)) {
			std::cout << "Something went wrong during loading" << "\n" << reader.getFormatedErrorMessages();
			removeSongFiles(song.storageLocation, strategy);
			return 0;
		}

		file.close();

		//std::cout << root;

		//Json::Value songs = root["songs"];

		std::transform(song.songName.begin(), song.songName.end(), song.songName.begin(), [](unsigned char c) {
			return std::tolower(c);
			});

		if(root["songs"][song.songName].isNull()) {
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