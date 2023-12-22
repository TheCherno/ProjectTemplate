#include "Backend.h"
#include <string>
#include <Windows.h>
#include <sstream>

#include "MusicTypes.h"

int main()
{
	Backend::reloadDirectory();
	std::string url;

	Song currentSong;

	std::string action;
	std::string userInput;

	Backend::setVolume(5);

	while (true) {
		Backend::printAndHandleInput();
	}
}