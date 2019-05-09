#ifndef SSAO_H
#define SSAO_H
#include <vector>
#include <random>
#include <GL/glew.h>
#include <glm/gtx/string_cast.hpp>
#include <glm/gtx/io.hpp>
#include "render_pass.h"
#include "Player.h"
using namespace glm;
//Handles the Screen Space Reflections Pass
struct SSAO {
public:
	SSAO(Player* p);
	void toScreen(GLuint* mainRenderTex, GLuint& depth);

	std::vector<vec4> quad_vert;
	std::vector<vec2> quad_uv;
	std::vector<uvec3> quad_faces;

	GLuint screen[1];
private:
	RenderDataInput input;
	RenderPass* render;
	Player* player;
	GLint depSten;
	GLuint norm_loc;
	GLuint ray_loc;
	GLuint randomNoise_loc;
	GLuint randomNoise;
	GLuint framebuffer;
	GLuint depth;
	GLenum DrawBuffers[1] = { GL_COLOR_ATTACHMENT0 };
	std::uniform_real_distribution<float> randomFloats = std::uniform_real_distribution<float>(0.0, 1.0);
	std::default_random_engine defRand;
	std::vector<vec3> SSAOSeed;
	const char* screen_frag =
#include "ssao.frag"
		;
	const char* screen_vert =
#include "screen.vert"
		;
};

#endif