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
#include "SSAO.h"
#include "Merge.h"

int main(int argc, char* argv[]) {
	discordInit();
	updateDiscordPresence();
	initSDL();

	//FrameBuffer setup
	GLuint FrameBuffer, depth;
	GLuint mainRenderTex[5] = { 1,1,1,1,1 };
	GLenum DrawBuffers[5] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2, GL_COLOR_ATTACHMENT3, GL_COLOR_ATTACHMENT4 };
	frameBufferSetup(FrameBuffer, mainRenderTex, depth, DrawBuffers, windowWidth, windowHeight, 5);

	Screen screen = Screen(&depth);
	Player player;
	//Class that'll hold all our objects
	vec4 light = vec4(0.0, 10.0, 0.0, 1.0);
	SSR ssr = SSR(&player);
	SSAO ssao = SSAO(&player);
	Merge merge = Merge();
	World world = World(&player, &light);

	while (!SDL_QuitRequested() && !player.quit) {
		player.update();
		toTextureSetup(windowWidth, windowHeight, FrameBuffer);
		world.toScreen(windowWidth * renderScale, windowWidth * renderScale);
		ssr.toScreen(mainRenderTex, depth);
		ssao.toScreen(mainRenderTex, depth);
		merge.toScreen(mainRenderTex[0], ssr.screen[0], ssao.screen[0], windowWidth, windowHeight);

		//screen.toScreen(ssr.screen[0], depth, windowWidth, windowHeight);
		screen.toScreen(merge.screen[0], depth, windowWidth, windowHeight);

		SDL_GL_SwapWindow(window);
	}
	cleanUp();
	Discord_Shutdown();
	return 0;
}