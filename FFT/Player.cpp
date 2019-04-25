// Have to know screen data
#include "Player.h"

void Player::update() {
	takeInput();
	glm::vec3 up = glm::vec3(0, 1, 0);
	glm::vec3 right = glm::vec3(1, 0, 0);
	camera = rotate(glm::mat4(), axis.x, up);
	camera = rotate(camera, axis.y, right);
	view = inverse(camera);
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
				playerX = -1;
				break;
			case SDLK_ESCAPE:
				quit = true;
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
					playerX = 0;
				break;
			}
			break;
		case SDL_MOUSEMOTION:
			std::cout << "X: " << event.motion.xrel << " Y: " << event.motion.yrel << std::endl;
		default:
			break;
		}
	}
}
