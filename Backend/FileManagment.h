#pragma once
#include <json\json.h>
#include <iostream>
#include <fstream>
#include <filesystem>
#include <string>
#include <stdexcept>

std::string convertToWav(std::string filePath);
void reloadDirectory(Json::Value& loadedDirectory);