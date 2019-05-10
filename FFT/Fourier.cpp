#include "Fourier.h"
#include "Player.h"

void Fourier::toScreen(int width, int height) {
	render->setup();
	glStencilFunc(GL_ALWAYS, 0, 0xFF);

	glViewport(0, 0, width, height);
	CHECK_GL_ERROR(glDrawElementsInstanced(GL_TRIANGLES,
		faces.size() * 3,
		GL_UNSIGNED_INT, 0, 7));
}

Fourier::Fourier(Player* p, std::vector<vec4>* l) {
	verts.push_back(glm::vec4(0, 0, 0, 1.0));//BL 0
	verts.push_back(glm::vec4(3, 0, 0, 1.0));//TL 1
	verts.push_back(glm::vec4(3, 3, 0, 1.0));//TL 2
	verts.push_back(glm::vec4(0, 3, 0, 1.0));//TR 3
	verts.push_back(glm::vec4(0, 0, 3, 1.0));//BL 4
	verts.push_back(glm::vec4(3, 0, 3, 1.0));//TL 5
	verts.push_back(glm::vec4(3, 3, 3, 1.0));//TL 6 
	verts.push_back(glm::vec4(0, 3, 3, 1.0));//TR 7
	//Front?
	faces.push_back(glm::uvec3(3, 1, 0));
	faces.push_back(glm::uvec3(2, 1, 3));
	//Back
	faces.push_back(glm::uvec3(2, 5, 1));
	faces.push_back(glm::uvec3(6, 5, 2));
	//Left
	faces.push_back(glm::uvec3(6, 4, 5));
	faces.push_back(glm::uvec3(7, 4, 6));
	//Right?
	faces.push_back(glm::uvec3(7, 0, 4));
	faces.push_back(glm::uvec3(3, 0, 7));
	//Top?
	faces.push_back(glm::uvec3(7, 2, 3));
	faces.push_back(glm::uvec3(6, 2, 7));
	//Bot?
	faces.push_back(glm::uvec3(0, 5, 4));
	faces.push_back(glm::uvec3(1, 5, 0));
	input = new RenderDataInput();
	input->assignIndex(faces.data(), faces.size(), 3);
	input->assign(0, "vertex_position", verts.data(), verts.size(), 4, GL_FLOAT);

	for (int i = -3; i <= 3; i++) {
		locations.push_back(vec3((float)i * 3, 0.0, -20.0));
	}
	soundPos = vec3(0.0, 0.0, -20.0);

	player = p;
	this->light = l;
	std::function <mat4()> proj_data = [this]() { return player->projection; };
	std::function <mat4()> view_data = [this]() { return player->view; };
	std::function <std::vector<vec4>()> light_data = [this]() { return *light; };
	std::function <int()> light_count = [this]() { return light->size(); };
	std::function <vec3()> pos_data = [this]() { return player->playerPos; };
	std::function <std::vector<float>()> buck_data = []() { return buckets; };
	std::function <std::vector<vec3>()> loc_data = [this]() { return locations; };
	std::function <float()> spec_data = [this]() { return spec; };

	auto proj = make_uniform("projection", proj_data);
	auto view = make_uniform("view", view_data);
	auto light_uni = make_uniform("light_position", light_data);
	auto light_c_uni = make_uniform("light_count", light_count);
	auto p_pos = make_uniform("camera_position", pos_data);
	auto bucket = make_uniform("buckets", buck_data);
	auto loc_uni = make_uniform("trans", loc_data);
	auto specs = make_uniform("specular", spec_data);

	render = new RenderPass(-1, *input,
		{ fourier_vert, fourier_geom, fourier_frag },
		{ proj, view, light_uni, light_c_uni, p_pos, bucket, loc_uni },
		{ "fragment_color", "vs_Normal", "world_Pos", "spec" });
}
