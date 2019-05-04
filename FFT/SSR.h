#ifndef SSR_H
#define SSR_H
#include <vector>
#include <GL/glew.h>
#include <glm/gtx/string_cast.hpp>
#include <glm/gtx/io.hpp>
#include "render_pass.h"
using namespace glm;
//Handles the Screen Space Reflections Pass
struct SSR {
public:
	SSR();
	void toScreen(GLuint* mainRenderTex, GLuint& depth);

	std::vector<vec4> quad_vert;
	std::vector<vec2> quad_uv;
	std::vector<uvec3> quad_faces;

	GLuint screen[1];
private:
	RenderDataInput input;
	RenderPass* render;
	GLint tex_loc;
	GLint depSten;
	GLint spec_loc;
	GLuint framebuffer;
	GLuint depth;
	GLenum DrawBuffers[1] = { GL_COLOR_ATTACHMENT0 };
	const char* screen_frag =
#include "ssr.frag"
		;
	const char* screen_vert =
#include "screen.vert"
		;
};

#endif