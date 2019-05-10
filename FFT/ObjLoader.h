#ifndef OBJL_H
#define OBJL_H
#include <vector>
#include <GL/glew.h>
#include <glm/gtx/string_cast.hpp>
#include <glm/gtx/io.hpp>
using namespace glm;
//Handles the Screen Space Reflections Pass
bool loadOBJ(const char* path, std::vector<vec4>& verts, std::vector<vec2>& uv, std::vector<vec3>& norms);
#endif