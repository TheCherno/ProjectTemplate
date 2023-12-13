#pragma once
#include <string>
#include <iostream>
#include <filesystem>

std::string getDownloadDirectory() {
	std::filesystem::path path = std::filesystem::current_path() / "MusicLibrary" / "%(title)s.%(ext)s";
	return path.string();
}