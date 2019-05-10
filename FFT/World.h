#ifndef WORLD_H
#define WORLD_H
#include <vector>
#include "Objects.h"
#include "Floor.h"
#include "Fourier.h"
#include <GL/glew.h>
#include <glm/gtx/string_cast.hpp>
#include <glm/gtx/io.hpp>
using namespace glm;

class World {
public:
	Floor floor;
	Fourier fourier;
	World(Player* p, std::vector<vec4>* light) : floor(p, light) , fourier(p, light) {};
	void toScreen(int width, int height) { floor.toScreen(width, height), fourier.toScreen(width, height); };
};
#endif // !WORLD_H