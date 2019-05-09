// Have to know screen data
#include "Player.h"

void Player::update() {
	takeInput();
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

	view = lookAt(playerPos, playerPos + forward, up);
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
