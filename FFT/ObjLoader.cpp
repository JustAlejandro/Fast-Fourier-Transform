#include "ObjLoader.h"
#include <iostream>

bool loadOBJ(const char* path, std::vector<vec4>& verts, std::vector<vec2>& uvs, std::vector<vec3>& norms) {
	std::vector<unsigned int> vertInd, uvInd, normInd;
	std::vector<vec3> temp_vert;
	std::vector<vec2> temp_uv;
	std::vector<vec3> temp_norm;

	FILE* file = fopen(path, "r");
	if (file == NULL) {
		std::cout << "OBJ borked" << std::endl;
		return false;
	}

	while (true) {
		char lineHeader[128];
		int res = fscanf(file, "%s", lineHeader);
		if (res == EOF) {
			break;
		}
		if (strcmp(lineHeader, "v") == 0) {
			vec3 vertex;
			fscanf(file, "%f %f %f\n", &vertex.x, &vertex.y, &vertex.z);
			temp_vert.push_back(vertex);
		}
		else if (strcmp(lineHeader, "vt") == 0) {
			vec2 uv;
			fscanf(file, "%f %f\n", &uv.x, &uv.y);
			temp_uv.push_back(uv);
		}
		else if (strcmp(lineHeader, "vn") == 0) {
			vec3 norm;
			fscanf(file, "%f %f %f\n", &norm.x, &norm.y, &norm.z);
			temp_norm.push_back(norm);
		}
		else if (strcmp(lineHeader, "f") == 0) {
			std::string v1, v2, v3;
			unsigned int vertI[3], uvI[3], normI[3];
			int matches = fscanf(file, "%d/%d/%d %d/%d/%d %d/%d/%d\n", &vertI[0], &uvI[0], &normI[0], &vertI[1], &uvI[1], &normI[1], &vertI[2], &uvI[2], &normI[2]);
			if (matches != 9) {
				std::cout << "File too complicated for parser" << std::endl;
				return false;
			}
			for (int i = 0; i < 3; i++) {
				vertInd.push_back(vertI[i]);
				uvInd.push_back(uvI[i]);
				normInd.push_back(uvI[i]);
			}
			for (unsigned int i = 0; i < vertInd.size(); i++) {
				unsigned int vertexIndex = vertInd[i];
				vec4 vert = vec4(temp_vert[vertexIndex - 1], 1.0);
				verts.push_back(vert);
			}
			for (unsigned int i = 0; i < normInd.size(); i++) {
				unsigned int normalIndex = normInd[i];
				vec3 norm = temp_norm[normalIndex - 1];
				norms.push_back(norm);
			}
			for (unsigned int i = 0; i < uvInd.size(); i++) {
				unsigned int uvIndex = uvInd[i];
				vec2 uv = temp_uv[uvIndex - 1];
				uvs.push_back(uv);
			}							
		}
	}
	std::cout << "out";
}
