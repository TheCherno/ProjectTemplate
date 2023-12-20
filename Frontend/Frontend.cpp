#include "Backend.h"
#include <string>

#include "MusicTypes.h"

int main()
{
	std::string url;
	std::cin.get();
	Song song;
	song.artist = "Eminem";
	song.hasLyricsAvailable = "false";
	song.sizeInBytes = -1;
	song.songName = "Lose Yourself";
	song.storageLocation = "None";

	Backend::addToSongDirectory(song);
	Backend::getSong("Test");
	std::cin >> url;
	

	Backend::downloadSong(url);

	std::cin.get();
}