#pragma once
#include <json\json.h>
#include <iostream>
#include <fstream>
#include <filesystem>
#include <string>
#include <stdexcept>
#include "MusicTypes.h"

std::string convertToWav(std::string filePath);
void reloadDirectory(Json::Value& loadedDirectory);
void removeSongFiles(std::string songPath);
int addToSongDirectory(Song song);