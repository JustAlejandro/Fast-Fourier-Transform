#ifndef CUSTOM_H
#define CUSTOM_H
#include "Objects.h"
#include "Player.h"
#include <vector>
#include <GL/glew.h>
#include <glm/gtx/string_cast.hpp>
#include <glm/gtx/io.hpp>
#include "render_pass.h"
using namespace glm;

class Custom : Object {
public:
	void toScreen(int width, int height);
	Custom(Player* p, std::vector<vec4>* l);
	virtual void iAm() { std::cout << "I am Object" << std::endl; };
private:
	std::vector<vec4> verts;
	std::vector<vec2> uv;
	std::vector<vec3> norms;
	RenderDataInput* input;
	RenderPass* render;
	Player* player;
	std::vector<vec4>* light;
	float spec = 0.0;

	const char* floor_frag =
#include "custom.frag"
		;
	const char* floor_geom =
#include "custom.geom"
		;
	const char* floor_vert =
#include "custom.vert"
		;
};
#endif // !FLOOR_H
