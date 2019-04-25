#ifndef PLAYER_H
#define PLAYER_H
#include <glm/gtx/string_cast.hpp>
#include <glm/gtx/io.hpp>
#include "SDLInit.h"

// This will handle our inputs as well as the Player's view and camera stuff.
using namespace glm;
struct Player {
	float FOV = 90.0f;
	float nearP = 1.0f;
	float farP = 400.0f;
	bool quit = false;
	mat4 camera;
	mat4 projection = glm::perspective((float)(FOV * M_PI / 180.0f), (float)windowWidth / windowHeight, nearP, farP);
	mat4 view;
	// x = Left/Right , y = Up/Down
	vec2 axis = vec2(0.0,0.0);
	
	// The Event Handler
	SDL_Event event;
	void update();

	// Handles direction of player movement
	int playerX = 0;
	int playerY = 0;
	vec3 playerPos = vec3(0.0,0.0,0.0);
private:
	void takeInput();
};


#endif // !PLAYER_H
