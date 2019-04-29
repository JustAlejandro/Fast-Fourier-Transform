#ifndef OBJECTS_H
#define OBJECTS_H
#include <vector>
#include <GL/glew.h>
#include <glm/gtx/string_cast.hpp>
#include <glm/gtx/io.hpp>
#include "render_pass.h"
using namespace glm;

class Object {
public:
	virtual void toScreen(int width, int height) {};
	Object() {};
	virtual ~Object() {};
protected:
};

#endif // !OBJECTS_H
