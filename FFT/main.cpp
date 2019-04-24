#include <GL/glew.h>
#include "discord.cpp"
#include "SDL.h"
#include <iostream>
#include <string>
#include <stdio.h>
#include "SDL_opengl.h"
#include "SDLInit.h"
#include "Screen.h"


int main(int argc, char* argv[]) {
	discordInit();
	updateDiscordPresence();

	initSDL();
	
	while (!SDL_QuitRequested()) {
		SDL_Delay(16);
		
	}

	cleanUp();
	Discord_Shutdown();
	return 0;
}