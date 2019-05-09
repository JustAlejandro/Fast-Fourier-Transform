#ifndef MERGE_H
#define MERGE_H
#include <vector>
#include <GL/glew.h>
#include <glm/gtx/string_cast.hpp>
#include <glm/gtx/io.hpp>
#include "render_pass.h"
using namespace glm;
//Generates the quad to render the texture on screen
struct Merge {
public:
	Merge();
	void toScreen(GLuint& mainRenderTex, GLuint& ssr, GLuint& ssao, int& width, int& height);

	std::vector<vec4> quad_vert;
	std::vector<vec2> quad_uv;
	std::vector<uvec3> quad_faces;
	GLuint screen[1];
private:
	RenderDataInput input;
	RenderPass* render;
	GLint tex_loc;
	GLint ssr_loc;
	GLint ssao_loc;

	GLuint framebuffer;
	GLuint depth;
	GLenum DrawBuffers[1] = { GL_COLOR_ATTACHMENT0 };
	const char* screen_frag =
#include "merge.frag"
		;
	const char* screen_vert =
#include "screen.vert"
		;
};

#endif