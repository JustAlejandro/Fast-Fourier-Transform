#ifndef FOURIER_H
#define FOURIER_H
#include "Objects.h"
#include "Player.h"
#include <vector>
#include <GL/glew.h>
#include <glm/gtx/string_cast.hpp>
#include <glm/gtx/io.hpp>
#include "render_pass.h"
using namespace glm;

class Fourier : Object {
public:
	void toScreen(int width, int height);
	Fourier(Player* p, vec4* light);
	virtual void iAm() { std::cout << "I am Object" << std::endl; };
private:
	std::vector<vec4> verts;
	std::vector<vec2> uv;
	std::vector<uvec3> faces;
	//Stores the offsets of each box
	std::vector<vec3> locations;
	RenderDataInput* input;
	RenderPass* render;
	Player* player;
	vec4* light;
	float spec = 0.0f;

	const char* fourier_frag =
#include "fourier.frag"
		;
	const char* fourier_geom =
#include "fourier.geom"
		;
	const char* fourier_vert =
#include "fourier.vert"
		;
};
#endif // !FLOOR_H
