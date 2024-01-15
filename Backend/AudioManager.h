#pragma once

#include "Backend.h"
#include "SFML/Audio.hpp"

#ifdef BACKEND_EXPORTS
#define BACKEND __declspec(dllexport)
#else
#define BACKEND __declspec(dllimport)
#endif

namespace Backend {
	class BACKEND AudioManager {
	private:
		sf::SoundBuffer buffer;
		sf::Sound sound;

	public:
		AudioManager() {

		}

		void playPog(std::string songName) {
			std::cout << "Angekomen!";
			Backend::initializeApplication(false);
			Backend::playSong(songName, buffer, sound);
		}
	};
	
	extern "C" BACKEND AudioManager* CreateAudioManager() {
		return new AudioManager();
	}

	extern "C" BACKEND void PlayPog(AudioManager* audio, std::string name) {
		std::cout << "aflaljfj";
		audio->playPog(name);
	}
}