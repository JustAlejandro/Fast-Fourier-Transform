#include "SSAO.h"
#include "SDLInit.h"
#include "glSetups.h"

SSAO::SSAO(Player* p) {
	player = p;
	//Setup verts
	quad_vert.push_back(glm::vec4(-1.0f, -1.0f, 0.5, 1.0f));
	quad_vert.push_back(glm::vec4(1.0f, -1.0f, 0.5, 1.0f));
	quad_vert.push_back(glm::vec4(1.0f, 1.0f, 0.5, 1.0f));
	quad_vert.push_back(glm::vec4(-1.0f, 1.0f, 0.5, 1.0f));
	//Setup UVs
	quad_uv.push_back(glm::vec2(0.0, 0.0));
	quad_uv.push_back(glm::vec2(1.0, 0.0));
	quad_uv.push_back(glm::vec2(1.0, 1.0));
	quad_uv.push_back(glm::vec2(0.0, 1.0));
	//Setup Faces
	quad_faces.push_back(glm::uvec3(0, 1, 2));
	quad_faces.push_back(glm::uvec3(2, 3, 0));

	//Generating our random vectors
	for (int i = 0; i < 144; i++) {
		glm::vec3 sample(
			randomFloats(defRand) * 2.0 - 1.0,
			randomFloats(defRand) * 2.0 - 1.0,
			randomFloats(defRand) * 2.0 - 1.0);
		sample = glm::normalize(sample);
		SSAOSeed.push_back(sample);
	}
	glGenTextures(1, &randomNoise);
	glBindTexture(GL_TEXTURE_2D, randomNoise);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, 12, 12, 0, GL_RGB, GL_FLOAT, &SSAOSeed[0]);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);


	//Setup RenderPass Input
	input.assignIndex(quad_faces.data(), quad_faces.size(), 3);
	input.assign(0, "vertex_position", quad_vert.data(), quad_vert.size(), 4, GL_FLOAT);
	input.assign(1, "tex_coord_in", quad_uv.data(), quad_uv.size(), 2, GL_FLOAT);

	std::function <mat4()> proj_data = [this]() { return player->projection; };
	auto proj = make_uniform("projection", proj_data);

	std::function <float()> aspec_data = [this]() {return aspect; };
	auto aspect_uni = make_uniform("aspect", aspec_data);

	//Setup RenderPass
	render = new RenderPass(-1, input,
		//Shaders
		{ screen_vert, nullptr, screen_frag },
		//Uniforms
		{ proj, aspect_uni },
		//Outputs
		{ "fragment_color" });
	render->setup();
	depSten = glGetUniformLocation(render->sp_, "depSten");
	CHECK_GL_ERROR(glUniform1i(depSten, 0));
	norm_loc = glGetUniformLocation(render->sp_, "vs_Normals");
	CHECK_GL_ERROR(glUniform1i(norm_loc, 1));
	ray_loc = glGetUniformLocation(render->sp_, "vs_Ray");
	glUniform1i(ray_loc, 2);
	randomNoise_loc = glGetUniformLocation(render->sp_, "noise");
	glUniform1i(randomNoise_loc, 3);

	frameBufferSetup(framebuffer, screen, depth, DrawBuffers, windowWidth, windowHeight, 1);
}

void SSAO::toScreen(GLuint* mainRenderTex, GLuint& depth) {
	glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
	glViewport(0, 0, windowWidth * renderScale, windowHeight * renderScale);
	glDisable(GL_DEPTH_TEST);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	render->setup();
	//Depth
	glActiveTexture(GL_TEXTURE0 + 0);
	glBindTexture(GL_TEXTURE_2D, depth);
	//Normals
	glActiveTexture(GL_TEXTURE0 + 1);
	glBindTexture(GL_TEXTURE_2D, mainRenderTex[1]);
	//Rays
	glActiveTexture(GL_TEXTURE0 + 2);
	glBindTexture(GL_TEXTURE_2D, mainRenderTex[4]);
	//Randoms
	glActiveTexture(GL_TEXTURE0 + 3);
 	glBindTexture(GL_TEXTURE_2D, randomNoise);

	CHECK_GL_ERROR(glDrawElements(GL_TRIANGLES, quad_faces.size() * 3, GL_UNSIGNED_INT, 0));
}
