#ifndef FLOOR_H
#define FLOOR_H
#include "Objects.h"
#include "Player.h"
#include <vector>
#include <GL/glew.h>
#include <glm/gtx/string_cast.hpp>
#include <glm/gtx/io.hpp>
#include "render_pass.h"
using namespace glm;

class Floor : Object{
public:
	void toScreen(int width, int height);
	Floor(Player* p, vec4* light);
	virtual void iAm() { std::cout << "I am Object" << std::endl; };
private:
	std::vector<vec4> verts;
	std::vector<vec2> uv;
	std::vector<uvec3> faces;
	RenderDataInput* input;
	RenderPass* render;
	Player* player;
	vec4* light;
	float spec = 0.7;

	const char* floor_frag =
#include "floor.frag"
		;
	const char* floor_geom =
#include "floor.geom"
		;
	const char* floor_vert =
#include "floor.vert"
		;
};
#endif // !FLOOR_H
