#include "Backend.h"

#include "Settings.h"

#include "MusicTypes.h"
#include <FileManagment.h>

#include <thread>

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
	Playlist currentPlaylist;
	std::vector<Playlist> playlists;
	
	bool playingPlaylist = false;

	std::string action;
	std::string userInput;

	sf::SoundBuffer buffer;
	sf::Sound sound;

	SaveStrategy saveStrategy = MP3;
	bool m_debug;

	void initializeApplication(bool debug) {
		m_debug = debug;

		checkFFMPEGInstallation();
		reloadDirectory(loadedDirectory, playlists);
	}

	void printAndHandleInput() {

		system("cls");

		if (sound.getStatus() == sf::Sound::Stopped) {
			currentSong.isPlaying = false;
		}

		std::cout << "Currently playing: " + currentSong.songName + "\n";
		std::cout << "------------" << (currentSong.isPlaying ? "Playing" : "Paused") << "------------" << std::endl << std::endl;
		
		std::cout << "Type play <songName> to play a song" << std::endl;
		std::cout << "Type download <url> to download a song" << std::endl;
		std::cout << "Type volume <number between 0-100> to set the volume" << std::endl;
		std::cout << "Type pause to pause the song" << std::endl;
		std::cout << "Type resume to resume the song" << std::endl;

		std::cout << std::endl;

		std::cout << "Type playlist add <playlistName> <songName> to add a song to a playlist" << std::endl;
		std::cout << "Type playlists to view all available playlists" << std::endl;
		std::cout << "Type play <playlistName> to play a playlist" << std::endl;

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
			if (getPlaylist(songName).name == Playlist().name) {
				playingPlaylist = false;
				std::transform(songName.begin(), songName.end(), songName.begin(), [](unsigned char c) {
					return std::tolower(c);
					});
				if (m_debug) std::cout << "Playing " + songName << std::endl;
				Backend::playSong(songName);
			}
			else {
				currentPlaylist = getPlaylist(songName);
				playingPlaylist = true;
				playSong(currentPlaylist.songs.at(0));
			}
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
			reloadDirectory(loadedDirectory, playlists);
		}
		else if (action == "list") {
			Json::Value songs = loadedDirectory["songs"];
			int count = 1;
			for (const auto& song : songs.getMemberNames()) {
				std::string songName = songs[song]["songName"].asString();
				if (songName == "") { continue; }
				std::cout << count << ".    " << songName << std::endl;
				count++;
			}
			awaitEnter();
		}
		else if (action == "playlist") {
			std::string secondAction;
			iss >> secondAction;
			if (secondAction == "") {
				std::cout << "Please specify a action" << std::endl;
				awaitEnter();
				return;
			}
			if (secondAction == "add") {
				std::string playlistName;
				iss >> playlistName;
				if (playlistName == "" || (getPlaylist(playlistName).name == "")) {
					std::cout << "usage: playlist add <playlistName> <songName>" << std::endl;
					awaitEnter();
					return;
				}
				std::string songName;
				std::getline(iss >> std::ws, songName);
				std::transform(songName.begin(), songName.end(), songName.begin(), [](unsigned char c) {
					return std::tolower(c);
					});
				Playlist playlist = getPlaylist(playlistName);
				if (playlist.name == "") {
					std::cout << "No playlist found named " << playlistName << ". Do you want to create it? Yes/No" << std::endl;
					std::string choice;
					std::getline(std::cin, choice);
					if (choice == "Yes" || choice == "yes" || choice == "y") {
						playlist = Playlist();
						playlist.name = playlistName;
					}
					else {
						return;
					}
				}
				playlist.songs.push_back(songName);
				addToPlaylistDirectory(playlist);
				reloadDirectory(loadedDirectory, playlists);
				std::cout << "Added " << songName << " to " << playlistName << std::endl;
				awaitEnter();
			}
		}

		action = "";
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

		if (m_debug) { std::cout << output; awaitEnter(); }

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
		Song song;

		std::string filePath = downloadSong(url);

		std::string songName;

		std::cout << "\nEnter the name of the song: \n";

		std::getline(std::cin, songName);

		song.songName = songName;

		std::string artist;

		std::cout << "Enter the artist: \n";
		std::getline(std::cin, artist);
		song.artist = artist;

		if (filePath == "-1") {
			std::cout << "Song already downloaded! Aborting...\n";
			awaitEnter();
			return;
		}
		std::string storageLocation;
		try {
			storageLocation = convertToAudio(filePath, saveStrategy, songName);
		}
		catch (const std::exception& e) {
			std::cout << e.what() << std::endl;
			awaitEnter();
			return;
		}

		song.storageLocation = storageLocation;

		if (m_debug) { std::cout << song.storageLocation; awaitEnter();}

		try {
			std::uintmax_t fileSize = std::filesystem::file_size(filePath);
			song.sizeInBytes = fileSize;
		}
		catch (const std::filesystem::filesystem_error& e) {
			std::cerr << "Error: " << e.what() << std::endl;
			removeSongFiles(song.storageLocation, saveStrategy);
			return;
		}


		if (!addToSongDirectory(song, saveStrategy)) {
			std::cout << "Song coulnt be added to directory, deleting may be required!\n";
			removeSongFiles(song.storageLocation, saveStrategy);
		}

		reloadDirectory(loadedDirectory, playlists);
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

	Playlist getPlaylist(std::string playlistName) {
		//std::cout << "checking if " << playlistName;
		//awaitEnter();
		auto it = std::find_if(playlists.begin(), playlists.end(), [&playlistName](const Playlist& playlist) {
			return playlist.name == playlistName;
		});

		if (it != playlists.end()) {
			return *it;
		}
		else {
			return Playlist();
		}
	}

	void playSong(std::string songName) {
		currentSong = getSong(songName);
		currentSong.isPlaying = true;

		std::string path = currentSong.storageLocation;

		if (!buffer.loadFromFile(path)) {
			std::cerr << "Failed to load audio file." << std::endl;
		}

		sound.sf::Sound::setBuffer(buffer);

		std::thread playbackThread([&]() {
			sound.play();
			while (sound.getStatus() == sf::Sound::Playing) {
				std::this_thread::sleep_for(std::chrono::milliseconds(100));
			}
			currentSong.isPlaying = false;
			if (playingPlaylist) {
				currentPlaylist.songIndex += 1;
				if (currentPlaylist.songIndex < currentPlaylist.songs.size()) {
					Song nextSong;
					nextSong = getNextSongFromPlaylist();
					playSong(nextSong.songName);
				}
				else {
					currentPlaylist.songIndex = 0;
					playingPlaylist = false;
				}
			}
		});

		while (currentSong.isPlaying) {
			printAndHandleInput();
			std::this_thread::sleep_for(std::chrono::milliseconds(100));
		}

		playbackThread.join();
	}

	Song getNextSongFromPlaylist() {
		return getSong(currentPlaylist.songs.at(currentPlaylist.songIndex));
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

