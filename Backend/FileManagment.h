#pragma once
#include <json\json.h>
#include <iostream>
#include <fstream>
#include <filesystem>
#include <string>
#include <stdexcept>
#include "MusicTypes.h"

enum SaveStrategy {
	MP3,
	WAV
};

std::string convertToAudio(std::string filePath, SaveStrategy strategy);
void reloadDirectory(Json::Value& loadedDirectory);
void removeSongFiles(std::string songPath);
int addToSongDirectory(Song song);
int convertWithFFMPEG(std::string fullPath, std::string pathWithoutExtension, std::string extension);