#include "Backend.h"

#include "Settings.h"

#include "MusicTypes.h"
#include <FileManagment.h>

#include "json\json.h"
#include <fstream>

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
	Song currentSong;

	std::string action;
	std::string userInput;

	sf::SoundBuffer buffer;
	sf::Sound sound;

	bool m_debug;

	void initializeApplication(bool debug) {
		m_debug = debug;

		checkFFMPEGInstallation();
		reloadDirectory(loadedDirectory);
	}

	void printAndHandleInput() {
		system("cls");
		std::cout << "Currently playing: " + currentSong.songName + "\n";
		std::cout << "------------" << (currentSong.isPlaying ? "Playing" : "Paused") << "------------" << std::endl << std::endl;
		
		std::cout << "Type play <songName> to play a song" << std::endl;
		std::cout << "Type download <url> to download a song" << std::endl;
		std::cout << "Type volume <number between 0-100> to set the volume" << std::endl;
		std::cout << "Type pause to pause the song" << std::endl;
		std::cout << "Type resume to resume the song" << std::endl;

		std::cout << std::endl;

		std::cout << "Type reload to reload the song library" << std::endl;
		std::cout << "Type list to list all available songs" << std::endl;


		std::getline(std::cin, userInput);

		std::istringstream iss(userInput);
		iss >> action;

		if (action == "download") {
			std::string url;
			iss >> url;
			Backend::addSong(url);
		}
		else if (action == "play") {
			std::string songName;
			std::getline(iss >> std::ws, songName);
			Backend::playSong(songName);
		}
		else if (action == "volume") {
			int volume;
			if (iss >> volume) {
				setVolume(volume);
			}
			else {
				float currentVolume = sound.getVolume();
				std::cout << "Current volume set to " << currentVolume << std::endl;
				awaitEnter();
			}
		}
		else if (action == "pause") {
			sound.pause();
			currentSong.isPlaying = false;
			currentSong.isPaused = true;
		}
		else if (action == "resume") {
			sound.play();
			currentSong.isPlaying = true;
			currentSong.isPaused = false;
		}
		else if (action == "reload") {
			reloadDirectory(loadedDirectory);
		}
		else if (action == "list") {
			Json::Value songs = loadedDirectory["songs"];
			int count = 1;
			for (const auto& song : songs.getMemberNames()) {
				std::string songName = songs[song]["songName"].asString();
				std::cout << count << ".    " << songName << std::endl;
				count++;
			}
			awaitEnter();
		}

		system("cls");
	}

	void awaitEnter() {
		std::cout << "Press enter to continue";
		std::string dummy;
		std::getline(std::cin, dummy);
	}

	std::string exec(const char* cmd) {
		char buffer[1024];
		std::string result = "";
		FILE* pipe = _popen((std::string(cmd) + " 2>&1").c_str(), "r");
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

			if (position != std::string::npos) {
				std::string filename = output.substr(position + strlen("[download] Destination: "));

				size_t pos = filename.find(".webm");

				// Check if ".webm" is found
				if (pos != std::string::npos) {
					// Extract the substring up to the position of ".webm"
					std::string result = filename.substr(0, pos + 5); // +5 to include ".webm"
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
		std::string storageLocation;
		try {
			storageLocation = convertToWav(filePath);
		}
		catch (const std::exception& e) {
			std::cout << e.what() << std::endl;
			return;
		}

		Song song;
		song.storageLocation = storageLocation;

		if (m_debug) { std::cout << song.storageLocation; awaitEnter();}

		std::string songName;

		std::cout << "\nEnter the name of the song: \n";
		
		std::getline(std::cin, songName);
	
		song.songName = songName;
		
		std::string artist;

		std::cout << "Enter the artist: \n";
		std::getline(std::cin, artist);
		song.artist = artist;

		try {
			std::uintmax_t fileSize = std::filesystem::file_size(filePath);
			song.sizeInBytes = fileSize;
		}
		catch (const std::filesystem::filesystem_error& e) {
			std::cerr << "Error: " << e.what() << std::endl;
			removeSongFiles(song.storageLocation);
			return;
		}


		if (!addToSongDirectory(song)) {
			std::cout << "Song coulnt be added to directory, deleting may be required!\n";
			removeSongFiles(song.storageLocation);
		}

		reloadDirectory(loadedDirectory);
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
			if(m_debug) awaitEnter();
			return song;
		}

		return song;
	}

	void playSong(std::string songName) {
		currentSong = getSong(songName);
		currentSong.isPlaying = true;

		std::string path = currentSong.storageLocation;

		if (!buffer.loadFromFile(path)) {
			std::cerr << "Failed to load audio file." << std::endl;
		}

		sound.sf::Sound::setBuffer(buffer);

		sound.play();

		while (sound.getStatus() == sf::Sound::Playing) {
			printAndHandleInput();
		}
	}

	void setVolume(int volume) {
		sound.setVolume(volume);
	}

	void checkFFMPEGInstallation() {
		std::string output = exec("C:\\FFmpeg\\ffmpeg\\bin\\ffmpeg.exe");

		if (m_debug) { std::cout << output; awaitEnter(); }

		if (output.find("not recognized") != std::string::npos || (output.find("cannot find the path specified") != std::string::npos) || (output.find("kann den angegebenen Pfad nicht finden") != std::string::npos)) {
			std::cout << "FFMPEG not installed, downloading...";
			const char* powershellScript = R"(
			# PowerShell script content here
			$ffmpegUrl = "https://github.com/BtbN/FFmpeg-Builds/releases/download/latest/ffmpeg-master-latest-win64-gpl.zip"
			$ffmpegDirectory = "C:\FFmpeg"
			$extractedFolder = "C:\FFmpeg\ffmpeg-master-latest-win64-gpl"
			
			# Create the directory if it doesn't exist
			New-Item -ItemType Directory -Force -Path $ffmpegDirectory | Out-Null
			
			# Download and extract FFmpeg
			Invoke-WebRequest -Uri $ffmpegUrl -OutFile "$ffmpegDirectory\ffmpeg.zip"
			Expand-Archive -Path "$ffmpegDirectory\ffmpeg.zip" -DestinationPath $ffmpegDirectory -Force
			
			Start-Sleep -Seconds 2

			# Rename the extracted folder to "ffmpeg"
			Rename-Item -Path $extractedFolder -NewName "ffmpeg" -Force
			
			# Add the bin directory to the PATH
			$binDirectory = Join-Path $ffmpegDirectory "ffmpeg\bin"
			[Environment]::SetEnvironmentVariable("Path", "$($env:Path);$binDirectory", [System.EnvironmentVariableTarget]::User)
			
			Write-Host "FFmpeg has been downloaded, unzipped, and added to the PATH."

			PAUSE
			)";

			const wchar_t* scriptFilePath = L"install_ffmpeg.ps1";
			std::ofstream scriptFile("install_ffmpeg.ps1");
			scriptFile << powershellScript;
			scriptFile.close();

			std::string output = exec("powershell.exe -ExecutionPolicy Bypass -File ./install_ffmpeg.ps1 -Verb RunAs");
			std::cout << output;
			if (m_debug) awaitEnter();
			remove("install_ffmpeg.ps1");

			std::cout << std::endl;

			std::cout << "Please restart the programm! Press Enter to exit";
			std::string dummy;
			std::getline(std::cin, dummy);
			exit(0);
		}
	}
}

