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

	while (true) {
		std::cout << "Currently playing: " + currentSong.songName + "\n";
		std::cout << "------------Playing------------" << std::endl << std::endl;

		std::cout << "Type play <songName> to play a song" << std::endl;
		std::cout << "Type download <url> to download a song" << std::endl;


		std::getline(std::cin, userInput);

		std::istringstream iss(userInput);
		iss >> action;

		//std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

		if(action == "download") {
			std::string url;
			iss >> url;
			Backend::addSong(url);
		}
		else if (action == "play") {
			std::string songName;
			iss >> songName;
			Song song = Backend::getSong(songName);


		}


		system("cls");
	}
}