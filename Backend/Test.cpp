#include "pch.h"
#include "Test.h"
#include "Backend.h"
#include <iostream>
#include <SFML\Audio.hpp>
#include "AL/al.h"
#include "FileManagment.h"
extern "C" BACKEND void printTest()
{
    std::cout << "Test";
    sf::SoundBuffer buffer;
    sf::Sound sound;
    Backend::initializeApplication(false);
    Backend::playSong("20:15");
    //std::cout << "Downloaded " << url;
}

BACKEND void playSong(const char* songName)
{
    std::cout << "Trying to play " << songName << std::endl;
    Backend::initializeApplication(false);
    Backend::playSong(songName);
}
