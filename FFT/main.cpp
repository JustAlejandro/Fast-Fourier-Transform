#include "discord.cpp"
#include "SDL.h"
#include "SDL_mixer.h"
#include <iostream>
#include <string>
using std::string;


static const char* BOOM_BOOM = "C:\\Users\\Alejandro\\Music\\ordinary.flac";

char backlog[4096];


int main(int argc, char* argv[])
{
	discordInit();
	updateDiscordPresence();

	string input = "";
	int result = 0;
	int flags = MIX_INIT_FLAC;

	if (SDL_Init(SDL_INIT_AUDIO) < 0) {
		printf("Failed to init SDL\n");
		exit(1);
	}

	if (flags != (result = Mix_Init(flags))) {
		printf("Could not initialize mixer (result: %d).\n", result);
		printf("Mix_Init: %s\n", Mix_GetError());
		exit(1);
	}


	Mix_OpenAudio(44100, AUDIO_S16SYS, 2, 4096);
	Mix_Music* music = Mix_LoadMUS(BOOM_BOOM);
	Mix_PlayMusic(music, 1);

	while (!SDL_QuitRequested()) {
		SDL_Delay(250);
	}

	Mix_FreeMusic(music);
	SDL_Quit();
	Discord_Shutdown();
	return 0;
}