#include "SSR.h"
#include "SDLInit.h"
#include "glSetups.h"

SSR::SSR(Player* p) {
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

	//Setup RenderPass Input
	input.assignIndex(quad_faces.data(), quad_faces.size(), 3);
	input.assign(0, "vertex_position", quad_vert.data(), quad_vert.size(), 4, GL_FLOAT);
	input.assign(1, "tex_coord_in", quad_uv.data(), quad_uv.size(), 2, GL_FLOAT);

	std::function <mat4()> proj_data = [this]() { return player->projection; };
	auto proj = make_uniform("projection", proj_data);

	//Setup RenderPass
	render = new RenderPass(-1, input,
		//Shaders
		{ screen_vert, nullptr, screen_frag },
		//Uniforms
		{proj},
		//Outputs
		{ "fragment_color" });
	render->setup();
	tex_loc = glGetUniformLocation(render->sp_, "sampler");
	glUniform1i(tex_loc, 0);
	depSten = glGetUniformLocation(render->sp_, "depSten");
	CHECK_GL_ERROR(glUniform1i(depSten, 1));
	spec_loc = glGetUniformLocation(render->sp_, "specular");
	glUniform1i(spec_loc, 2);
	norm_loc = glGetUniformLocation(render->sp_, "vs_Normals");
	glUniform1i(norm_loc, 3);
	ray_loc = glGetUniformLocation(render->sp_, "vs_Ray");
	glUniform1i(ray_loc, 4);

	frameBufferSetup(framebuffer, screen, depth, DrawBuffers, windowWidth, windowHeight);
}

void SSR::toScreen(GLuint* mainRenderTex, GLuint& depth) {
	glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
	glViewport(0, 0, windowWidth * renderScale, windowHeight * renderScale);
	glDisable(GL_DEPTH_TEST);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	render->setup();
	//Colors
	glActiveTexture(GL_TEXTURE0 + 0);
	glBindTexture(GL_TEXTURE_2D, mainRenderTex[0]);
	//Depth
	glActiveTexture(GL_TEXTURE0 + 1);
	glBindTexture(GL_TEXTURE_2D, depth);
	//Specular
	glActiveTexture(GL_TEXTURE0 + 2);
	glBindTexture(GL_TEXTURE_2D, mainRenderTex[3]);
	//Normals
	glActiveTexture(GL_TEXTURE0 + 3);
	glBindTexture(GL_TEXTURE_2D, mainRenderTex[1]);
	//Rays
	glActiveTexture(GL_TEXTURE0 + 4);
	glBindTexture(GL_TEXTURE_2D, mainRenderTex[4]);
	CHECK_GL_ERROR(glDrawElements(GL_TRIANGLES, quad_faces.size() * 3, GL_UNSIGNED_INT, 0));
}
