#ifndef FLOOR_H
#define FLOOR_H
#include "Objects.h"
#include <vector>
#include <GL/glew.h>
#include <glm/gtx/string_cast.hpp>
#include <glm/gtx/io.hpp>
#include "render_pass.h"
using namespace glm;

class Floor : Object{
public:
	void toScreen(int width, int height);
private:
	
};
#endif // !FLOOR_H
