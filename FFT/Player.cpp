// Have to know screen data
#include "Player.h"

Player::Player() {
	//Generating our random vectors
	for (int i = 0; i < 100; i++) {
		glm::vec3 sample(
			randomFloats(defRand) * 2.0 - 1.0,
			randomFloats(defRand) * 2.0 - 1.0,
			randomFloats(defRand) * 2.0 - 1.0);
		sample = glm::normalize(sample) / 300.0f;
		jitter.push_back(sample);
	}
}

void Player::update() {
	takeInput();
	time = (time + 1) % 100;
	glm::vec3 up = glm::vec3(0, 1.0, 0);
	glm::vec3 right = glm::vec3(1.0, 0, 0);
	glm::vec3 forward = glm::vec3(0, 0, -1.0);

	mat4 rot = rotate(glm::mat4(1.0), axis.x, up) * rotate(mat4(1.0), axis.y, right);
	orient = rot;
	forward = normalize(rot * vec4(forward, 0.0));
	up = normalize(rot * vec4(up, 0.0));
	right = normalize(rot * vec4(right, 0.0));

	playerPos += right * (float)playerX + forward * (float)playerY;
	playPosition = playerPos;
	vec3 pos = playerPos + jitter.at(time);
	view = lookAt(pos, pos + forward, up);
}

void Player::takeInput() {
	while (SDL_PollEvent(&event)) {
		switch (event.type) {
		case SDL_KEYDOWN:
			switch (event.key.keysym.sym) {
			default:
				break;
			case SDLK_a:
				playerX = -1;
				break;
			case SDLK_d:
				playerX = 1;
				break;
			case SDLK_w:
				playerY = 1;
				break;
			case SDLK_s:
				playerY = -1;
				break;
			case SDLK_ESCAPE:
				quit = true;
				break;
			case SDLK_1:
				ssr = !ssr;
				break; 
			case SDLK_2:
				ssao = !ssao;
				break;
			case SDLK_3:
				taa = !taa;
				break;
			}
			break;
		case SDL_KEYUP:
			switch (event.key.keysym.sym) {
			default:
				break;
			case SDLK_a:
				if (playerX < 0)
					playerX = 0;
				break;
			case SDLK_d:
				if (playerX > 0)
					playerX = 0;
				break;
			case SDLK_w:
				if (playerY > 0)
					playerY = 0;
				break;
			case SDLK_s:
				if (playerY < 0)
					playerY = 0;
				break;
			}
			break;
		case SDL_MOUSEMOTION:
			axis.x -= event.motion.xrel * 0.005;
			axis.y += -event.motion.yrel * 0.005;
			break;
		default:
			break;
		}
	}
}
