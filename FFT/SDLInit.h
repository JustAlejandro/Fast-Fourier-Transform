#ifndef SDL_INIT_H
#define SDL_INIT_H

#include "SDL.h"
#include "SDL_mixer.h"
#include "SDL_opengl.h"
#include "FFT.h"
#include <debuggl.h>
#include <string>
#include <iostream>
#include <vector>
#include <glm/gtx/string_cast.hpp>
#include <glm/gtx/io.hpp>

#define GL3_PROTOTYPES 1

extern int windowWidth;
extern int windowHeight;
extern float aspect;
extern std::vector<float> buckets;
extern float resScale;
extern float renderScale;
extern SDL_Window* window;
extern glm::mat4 orient;
extern float audioDist;
extern glm::vec3 playPosition;
extern glm::vec3 soundPos;

bool initSDL();
bool setOpenGLAttributes();
void cleanUp();

void checkSDLError(int line);

void passThrough(int chan, void* stream, int len, void* udata);

void normalizeBuckets();

void fixBuckets(double a, int id);

#endif // !SDL_INIT_H
