#include <GL/glew.h>
#include "discord.cpp"
#include "SDL.h"
#include <iostream>
#include <string>
#include <stdio.h>
#include "SDL_opengl.h"
#include "SDLInit.h"
#include "Screen.h"
#include "glSetups.h"

int main(int argc, char* argv[]) {
	discordInit();
	updateDiscordPresence();
	initSDL();

	//FrameBuffer setup
	GLuint FrameBuffer, mainRenderTex, depthrenderbuffer;
	GLenum DrawBuffers[1] = { GL_COLOR_ATTACHMENT0 };
	frameBufferSetup(FrameBuffer, mainRenderTex, depthrenderbuffer, DrawBuffers, windowWidth, windowHeight);

	Screen screen;

	while (!SDL_QuitRequested()) {
		toTextureSetup(windowWidth, windowHeight, FrameBuffer);
		screen.toScreen(mainRenderTex, windowWidth, windowHeight);

		SDL_GL_SwapWindow(window);
	}

	cleanUp();
	Discord_Shutdown();
	return 0;
}