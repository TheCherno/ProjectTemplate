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

	std::string convertToWav(std::string filePath) {

		size_t dotPosition = filePath.find_last_of('.');

		std::string fileNameWithoutExtension;

		if (dotPosition != std::string::npos) {
			fileNameWithoutExtension = filePath.substr(0, dotPosition);
		}

		int result = system(("ffmpeg -i \"" + filePath + "\" -vn \"" + fileNameWithoutExtension + ".wav\"").c_str());

		if (result == 0) {
			return fileNameWithoutExtension + ".wav";
		}
		else {
			throw std::runtime_error("Error converting to .wav");
		}
	}
