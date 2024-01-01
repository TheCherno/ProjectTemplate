#include "Backend.h"
#include <string>
#include <Windows.h>

#include "MusicTypes.h"
#include <FileManagment.h>
#include <iostream>
#include<sstream>

int main()
{
	std::string debug = "false";
	std::cout << "Enable debug print? false or true\n";
	std::getline(std::cin , debug);

	Backend::initializeApplication(debug == "true");
	std::string url;

	Song currentSong;

	std::string action;
	std::string userInput;

	Backend::setVolume(5);

	while (true) {
		Backend::printAndHandleInput();
	}
}