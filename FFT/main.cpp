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
#include "Player.h"
#include "World.h"
#include "SSR.h"

int main(int argc, char* argv[]) {
	discordInit();
	updateDiscordPresence();
	initSDL();

	//FrameBuffer setup
	GLuint FrameBuffer, depth;
	GLuint mainRenderTex[4] = { 1,1,1,1 };
	GLenum DrawBuffers[4] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2, GL_COLOR_ATTACHMENT3 };
	frameBufferSetup(FrameBuffer, mainRenderTex, depth, DrawBuffers, windowWidth, windowHeight, 4);

	Screen screen = Screen(&depth);
	SSR ssr;
	Player player;
	//Class that'll hold all our objects
	vec4 light = vec4(0.0, 10.0, 0.0, 1.0);
	World world = World(&player, &light);

	while (!SDL_QuitRequested() && !player.quit) {
		player.update();
		toTextureSetup(windowWidth, windowHeight, FrameBuffer);
		world.toScreen(windowWidth * renderScale, windowWidth * renderScale);
		ssr.toScreen(mainRenderTex, depth);

		screen.toScreen(ssr.screen[0], depth, windowWidth, windowHeight);

		SDL_GL_SwapWindow(window);
	}
	cleanUp();
	Discord_Shutdown();
	return 0;
}