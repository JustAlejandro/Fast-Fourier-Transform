#ifndef PLAYER_H
#define PLAYER_H
#include <glm/gtx/string_cast.hpp>
#include <glm/gtx/io.hpp>
#include "SDLInit.h"
#include <random>

// This will handle our inputs as well as the Player's view and camera stuff.
using namespace glm;
class Player {
public:
	Player();
	float FOV = 45.0f;
	float nearP = 0.5f;
	float farP = 600.0f;
	bool quit = false;
	mat4 camera;
	mat4 projection = glm::perspective(FOV, aspect, nearP, farP);
	mat4 view;
	// x = Left/Right , y = Up/Down
	vec2 axis = vec2(0.0,0.0);
	
	// The Event Handler
	SDL_Event event;
	void update();

	// Handles direction of player movement
	int playerX = 0;
	int playerY = 0;
	vec3 playerPos = vec3(0.0,2.0,0.0);
private:
	int time = 0;
	void takeInput();
	std::uniform_real_distribution<float> randomFloats = std::uniform_real_distribution<float>(0.0, 1.0);
	std::default_random_engine defRand;
	std::vector<vec3> jitter;
};


#endif // !PLAYER_H
