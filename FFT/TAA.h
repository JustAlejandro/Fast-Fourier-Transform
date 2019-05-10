#ifndef TAA_H
#define TAA_H
#include <vector>
#include <GL/glew.h>
#include <glm/gtx/string_cast.hpp>
#include <glm/gtx/io.hpp>
#include "render_pass.h"
#include "Player.h"
#define HIST 8
using namespace glm;

struct TAA {
public:
	TAA(Player* p);
	void setHistory(GLuint& image, GLuint& world);
	void toScreen(GLuint& mainRenderTex, GLuint& worldPos, int& width, int& height);

	std::vector<vec4> quad_vert;
	std::vector<vec2> quad_uv;
	std::vector<uvec3> quad_faces;
	GLuint screen[1];
	GLuint history;
	GLuint hisWorld;
	std::vector<mat4> viewHist;
private:
	RenderDataInput input;
	RenderPass* render;
	GLint tex_loc;
	GLint world_loc;
	Player* player;

	GLuint framebuffer;
	GLuint depth;
	GLenum DrawBuffers[1] = { GL_COLOR_ATTACHMENT0 };
	const char* screen_frag =
#include "taa.frag"
		;
	const char* screen_vert =
#include "screen.vert"
		;
};

#endif