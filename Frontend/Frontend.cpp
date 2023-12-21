#include "Backend.h"
#include <string>
#include <Windows.h>

#include "MusicTypes.h"

int main()
{
	std::string url;

	std::cin >> url;
	
	Backend::addSong(url);

	std::cin.get();
}