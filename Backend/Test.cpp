#include "pch.h"
#include "Test.h"
#include "Backend.h"
#include <iostream>
#include <SFML\Audio.hpp>
#include "AL/al.h"

sf::SoundBuffer sb;

extern "C" BACKEND void printTest()
{
    //sf::SoundBuffer buffer;
    /*if (!buffer.loadFromFile("C:\\Users\\Kreiseljustus\\Desktop\\SFML\\SFML-2.6.1\\examples\\sound\\resources\\ding.mp3")) {
        // Print error message or handle the error
        std::cerr << "Failed to load audio file:";
    }*/
    std::cout << "1";
    //sf::SoundBuffer sf;
    //sf::Sound sound;
    
    //Backend::initializeApplication(true);
    std::cout << "Test\n";
}