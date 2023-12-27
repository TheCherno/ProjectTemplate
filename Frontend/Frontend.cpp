#include "Backend.h"
#include <string>
#include <Windows.h>
#include <sstream>

#include "MusicTypes.h"
#include <FileManagment.h>

int main()
{
	Backend::checkFFMPEGInstallation();
	std::string url;

	Song currentSong;

	std::string action;
	std::string userInput;

	Backend::setVolume(5);

	while (true) {
		Backend::printAndHandleInput();
	}
}