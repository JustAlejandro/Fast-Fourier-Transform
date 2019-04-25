#ifndef SDL_INIT_H
#define SDL_INIT_H

#include "SDL.h"
#include "SDL_mixer.h"
#include "SDL_opengl.h"
#include "FFT.h"
#include <debuggl.h>
#include <string>
#include <iostream>

#define GL3_PROTOTYPES 1

extern int windowWidth;
extern int windowHeight;
extern double buckets[7];
extern float resScale;
extern float renderScale;
extern SDL_Window* window;

bool initSDL();
bool setOpenGLAttributes();
void cleanUp();

void checkSDLError(int line);

void passThrough(int chan, void* stream, int len, void* udata);

void normalizeBuckets();

void fixBuckets(double a, int id);

#endif // !SDL_INIT_H
