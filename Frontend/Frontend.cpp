#include "Backend.h"
#include <string>

int main()
{
	std::string url;
	Backend::addToSongDirectory();
	std::cin >> url;

	Backend::downloadSong(url);

	std::cin.get();
}