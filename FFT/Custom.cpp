#include "Custom.h"
#include "Player.h"
#include "ObjLoader.h"

void Custom::toScreen(int width, int height) {
	render->setup();

	glViewport(0, 0, width, height);
	CHECK_GL_ERROR(glDrawElements(GL_TRIANGLE_STRIP,
		verts.size() / 3 * 0 +3,
		GL_UNSIGNED_INT, 0));
}

Custom::Custom(Player* p, std::vector<vec4>* l) {
	loadOBJ("C:\\Users\\Alejandro\\Documents\\dragon.obj", verts, uv, norms);

	input = new RenderDataInput();
	//input->assignIndex(faces.data(), faces.size(), 3);
	input->assign(1, "normal", norms.data(), norms.size(), 3, GL_FLOAT);
	input->assign(0, "vertex_position", verts.data(), verts.size(), 4, GL_FLOAT);

	player = p;
	this->light = l;
	std::function <mat4()> proj_data = [this]() { return player->projection; };
	std::function <mat4()> view_data = [this]() { return player->view; };
	std::function <std::vector<vec4>()> light_data = [this]() { return *light; };
	std::function <int()> light_count = [this]() { return light->size(); };
	std::function <vec3()> pos_data = [this]() { return player->playerPos; };
	std::function <float()> spec_data = [this]() { return spec; };


	auto proj = make_uniform("projection", proj_data);
	auto view = make_uniform("view", view_data);
	auto light_uni = make_uniform("light_position", light_data);
	auto light_c_uni = make_uniform("light_count", light_count);
	auto p_pos = make_uniform("camera_position", pos_data);
	auto specs = make_uniform("specular", spec_data);

	render = new RenderPass(-1, *input,
		{ floor_vert, floor_geom, floor_frag },
		{ proj, view, light_uni, p_pos, specs, light_c_uni },
		{ "fragment_color", "vs_Normal", "world_Pos", "spec" });
}
