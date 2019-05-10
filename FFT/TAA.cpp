#include "TAA.h"
#include "glSetups.h"
#include "SDLInit.h"

TAA::TAA(Player* p) {
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

	//Setup RenderPass
	render = new RenderPass(-1, input,
		//Shaders
		{ screen_vert, nullptr, screen_frag },
		//Uniforms
		{},
		//Outputs
		{ "fragment_color" });
	render->setup();
	tex_loc = glGetUniformLocation(render->sp_, "history[0]");
	glUniform1i(tex_loc, 0);

	frameBufferSetup(framebuffer, screen, depth, DrawBuffers, windowWidth, windowHeight, 1);

	//Frame history setup
	glGenTextures(1, &history);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D_ARRAY, history);
	glTexStorage3D(GL_TEXTURE_2D_ARRAY, 1, GL_RGBA32F_ARB, windowWidth, windowHeight, HIST);

	glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	//World History Setup
	glGenTextures(1, &hisWorld);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D_ARRAY, hisWorld);
	glTexStorage3D(GL_TEXTURE_2D_ARRAY, 1, GL_RGBA32F_ARB, windowWidth, windowHeight, HIST);

	glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
}

void TAA::setHistory(GLuint& image, GLuint& world) {
	for (int i = HIST-1; i > 0; i--) {
		CHECK_GL_ERROR(glCopyImageSubData(history, GL_TEXTURE_2D_ARRAY, 0, 0, 0, i-1,
			history, GL_TEXTURE_2D_ARRAY, 0, 0, 0, i, windowWidth, windowHeight, 1));

		CHECK_GL_ERROR(glCopyImageSubData(hisWorld, GL_TEXTURE_2D_ARRAY, 0, 0, 0, i-1,
			hisWorld, GL_TEXTURE_2D_ARRAY, 0, 0, 0, i, windowWidth, windowHeight, 1));
	}
	CHECK_GL_ERROR(glCopyImageSubData(image, GL_TEXTURE_2D, 0, 0, 0, 0,
		history, GL_TEXTURE_2D_ARRAY, 0, 0, 0, 0, windowWidth, windowHeight, 1));

	CHECK_GL_ERROR(glCopyImageSubData(world, GL_TEXTURE_2D, 0, 0, 0, 0,
		hisWorld, GL_TEXTURE_2D_ARRAY, 0, 0, 0, 0, windowWidth, windowHeight, 1));
}

void TAA::toScreen(GLuint& mainRenderTex, GLuint& worldPos, int& width, int& height) {
	glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
	glViewport(0, 0, width, height);
	glDepthFunc(GL_ALWAYS);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	render->setup();

	setHistory(mainRenderTex, worldPos);
	glActiveTexture(GL_TEXTURE0 + 0);
	glBindTexture(GL_TEXTURE_2D_ARRAY, history);
	
	CHECK_GL_ERROR(glDrawElements(GL_TRIANGLES, quad_faces.size() * 3, GL_UNSIGNED_INT, 0));
}
