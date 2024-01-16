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

		~AudioManager() {
			
		}

		void playSong(const char* name) {
			Backend::initializeApplication(false);
			Backend::playSong(name, buffer, sound);
		}

		void test() {
			std::cout << "Void in class";
		}
	};
	
	extern "C" BACKEND AudioManager* CreateAudioManager() {
		AudioManager* audio = new AudioManager();
		std::cout << "AudioManager created: " << audio << std::endl;
		return audio;
	}

	extern "C" BACKEND void AudioManager_PlayPog(AudioManager* audio, const char* name) {
		std::cout << "Received " << name << std::endl;
		audio->playSong(name);
		audio->test();
	}
	extern "C" BACKEND void DestroyAudioManager(AudioManager * audio) {
		std::cout << "Destroying AudioManager: " << audio << std::endl;
		delete audio;
	}
}