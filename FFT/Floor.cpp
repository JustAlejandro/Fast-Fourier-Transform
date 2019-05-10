#include "Floor.h"
#include "Player.h"

const float kFloorXMin = -1000.0;
const float kFloorXMax = 1000.0;
const float kFloorY = 0.0;
const float kFloorZMax = 1000.0;
const float kFloorZMin = -1000.0;

void Floor::toScreen(int width, int height) {
	time = time + 1.0;
	render->setup();
	glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
	glStencilFunc(GL_ALWAYS, 1, 0xFF);

	glViewport(0, 0, width, height);
	CHECK_GL_ERROR(glDrawElements(GL_TRIANGLES,
		faces.size() * 3,
		GL_UNSIGNED_INT, 0));
}

Floor::Floor(Player* p, vec4* l) {
	verts.push_back(glm::vec4(kFloorXMin, kFloorY, kFloorZMax, 1.0f));
	verts.push_back(glm::vec4(kFloorXMax, kFloorY, kFloorZMax, 1.0f));
	verts.push_back(glm::vec4(kFloorXMax, kFloorY, kFloorZMin, 1.0f));
	verts.push_back(glm::vec4(kFloorXMin, kFloorY, kFloorZMin, 1.0f));
	faces.push_back(glm::uvec3(0, 1, 2));
	faces.push_back(glm::uvec3(2, 3, 0));

	input = new RenderDataInput();
	input->assignIndex(faces.data(), faces.size(), 3);
	input->assign(0, "vertex_position", verts.data(), verts.size(), 4, GL_FLOAT);

	player = p;
	this->light = l;
	std::function <mat4()> proj_data = [this]() { return player->projection; };
	std::function <mat4()> view_data = [this]() { return player->view; };
	std::function <vec4()> light_data = [this]() { return *light; };
	std::function <vec3()> pos_data = [this]() { return player->playerPos; };
	std::function <float()> spec_data = [this]() { return spec; };
	std::function <float()> time_data = [this]() { return time; };


	auto proj = make_uniform("projection", proj_data);
	auto view = make_uniform("view", view_data);
	auto light_uni = make_uniform("light_position", light_data);
	auto p_pos = make_uniform("camera_position", pos_data);
	auto specs = make_uniform("specular", spec_data);
	auto time_uni = make_uniform("time", time_data);

	render = new RenderPass(-1, *input,
		{ floor_vert, floor_geom, floor_frag },
		{ proj, view, light_uni, p_pos, specs, time_uni },
		{ "fragment_color", "vs_Normal", "world_Pos", "spec" });
}
