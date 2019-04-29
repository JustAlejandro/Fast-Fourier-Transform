#include "SDL.h"
#include <glew.h>
#include <deque>
#include "SDL_mixer.h"
#include "SDL_opengl.h"
#include "FFT.h"
#include <debuggl.h>
#include <string>
#include <iostream>
#include "SDLInit.h"

//Program name
const char* name = "JustRender";
//Song Path
static const char* BOOM_BOOM = "C:\\Users\\Alejandro\\Music\\ordinary.wav";
//Holds the music frequency values, sorted into buckets to display later
std::vector<float> bucketDirty(7, 0.0);
std::vector<float> buckets(7,0.0);
std::deque<std::vector<float>> bucketHistory;

//Orientation and vector relative to audio source
float audioDist = 0.0f;
glm::mat4 orient(1.0);
glm::vec3 playPosition;
glm::vec3 soundPos;

//Main SDL window
SDL_Window* window;
Mix_Chunk* sound;

//This will store all the display info about our system
//Using this later to change other resolution stuff
SDL_DisplayMode dis;
int windowWidth = 0;
int windowHeight = 0;

//Amount to scale each dimension for the window
float resScale = 0.5;

//Amount to scale the rendered image relative to dis parameters
//Plan is to maybe use this for Dynamic Resolution Scaling
float renderScale = 1.0;

//SDL context Handler
SDL_GLContext mainContext;

bool initSDL() {
	if (SDL_Init(SDL_INIT_EVERYTHING) < 0) {
		std::cout << "Couldn't start up SDL." << std::endl;
		return false;
	}

	setOpenGLAttributes();

	SDL_DisplayMode dis;
	SDL_GetCurrentDisplayMode(0, &dis);
	windowWidth = dis.w * resScale;
	windowHeight = dis.h * resScale;

	//Init window to be resScale * resolution
	window = SDL_CreateWindow(name, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
		windowWidth, windowHeight, SDL_WINDOW_OPENGL);

	if (!window) {
		std::cout << "Couldn't create a window" << std::endl;
		checkSDLError(__LINE__);
		return false;
	}

	//Create the opengl context
	mainContext = SDL_GL_CreateContext(window);
	if (mainContext == NULL) {
		std::cout << "MAIN CONTEXT COULD NOT BE CREATED" << std::endl;
	}

	CHECK_GL_ERROR(SDL_GL_MakeCurrent(window, mainContext));

	//Initialize GLEW
	glewExperimental = GL_TRUE;
	GLenum glewError = glewInit();
	if (glewError != GLEW_OK) {
		std::cout << "Error initializing GLEW! %s\n" << glewGetErrorString(glewError) << std::endl;

	}
	//Sometimes glew just throws errors, so we'll clear them
	glGetError();
	//const GLubyte* renderer = glGetString(GL_RENDERER);  // get renderer string
	//const GLubyte* version = glGetString(GL_VERSION);    // version as a string
	//std::cout << "Renderer: " << renderer << "\n";
	//std::cout << "OpenGL version supported:" << version << std::endl;
	//V-sync
	if (SDL_GL_SetSwapInterval(1) < 0) {
		printf("Warning: Unable to set VSync! SDL Error: %s\n", SDL_GetError());
	}

	Mix_OpenAudio(22050, AUDIO_S16SYS, 2, 1024);
	sound = Mix_LoadWAV(BOOM_BOOM);
	if (sound == NULL) {
		std::cout << "Something Broke" << std::endl;
		return 0;
	}

	int channel = Mix_PlayChannel(0, sound, -1);
	Mix_RegisterEffect(MIX_CHANNEL_POST, passThrough, NULL, NULL);

	// Capture and hold the mouse position
	CHECK_GL_ERROR(SDL_SetRelativeMouseMode(SDL_TRUE));

	return true;
}

bool setOpenGLAttributes() {
	//Set the openGL version

	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);

	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
	return true;
}

void cleanUp() {
	SDL_GL_DeleteContext(mainContext);
	SDL_DestroyWindow(window);

	Mix_FreeChunk(sound);
	Mix_CloseAudio();

	SDL_Quit();
}

void checkSDLError(int line = -1) {
	std::string error = SDL_GetError();

	if (error != "") {
		std::cout << "SDL ERROR : " << error << std::endl;
		if (line != -1) {
			std::cout << "\nLine : " << line << std::endl;
		}

		SDL_ClearError();
	}
}


//********************************SDL Sound Setups***********************

//Doesn't change the sound, just passes it through so we can read the data
void passThrough(int chan, void* stream, int len, void* udata) {
	float value = 1.0f;
	short* p = (short*)stream;
	int length = len / 2;
	//Positional Audio
	audioDist = glm::distance(soundPos, playPosition);
	glm::vec3 toSound = glm::normalize(soundPos - playPosition);
	float LDrop = (glm::dot(glm::vec3(glm::vec4(1.0,0.0,0.0,0.0) * orient), toSound) + 1.0) / 2.0;
	float RDrop = (glm::dot(glm::vec3(glm::vec4(-1.0, 0.0, 0.0, 0.0) * orient), toSound) + 1.0) / 2.0;
	//Reset the buckets on every run
	for (int i = 0; i < 7; i++) {
		bucketDirty[i] = 0.0;
	}

	/* Iterate over p 2 by 2 */
	for (int i = 0; i < length / 2; i++) {
		FFTdata[i] = (std::complex<double>)(short)(p[i * 2] * value);
		p[i * 2] = (short)(p[i * 2] * value) * (1.0 / (1.0 + 0.002 * audioDist * audioDist) * LDrop);
		p[i * 2 + 1] = (short)(p[i * 2 + 1] * value) * (1.0 / (1.0 + 0.002 * audioDist * audioDist) * RDrop);
	}

	CArray data(FFTdata, 2048);
	fft(data);
	double amplitude = -1000;
	int maxI = -1;
	// Why -512? Because that's around 6kHz and who cares about screeches
	for (int i = 0; i < data.size() - 256; i++) {
		fixBuckets(std::sqrt(data[i].real() * data[i].real() + data[i].imag() * data[i].imag()), (i * 22050 / 2) / (2048 / 2));
	}
	normalizeBuckets();
}

void normalizeBuckets() {
	bucketDirty[0] = bucketDirty[0] / 200;
	bucketDirty[1] = bucketDirty[1] / 150;
	bucketDirty[2] = bucketDirty[2] / 250;
	bucketDirty[3] = bucketDirty[3] / 200;
	bucketDirty[4] = bucketDirty[4] / 200;
	bucketDirty[5] = bucketDirty[5] / 500;
	bucketDirty[6] = bucketDirty[6] / 600;
	float max = -100.0;
	for (int i = 0; i < 7; i++) {
		if (bucketDirty[i] > max) max = bucketDirty[i];
	}
	for (int i = 0; i < 7; i++) {
		bucketDirty[i] = (bucketDirty[i] / max * 3.0);
	}
	bucketHistory.push_front(bucketDirty);
	if (bucketHistory.size() > 6) bucketHistory.resize(6);
	for (int i = 0; i < 7; i++) {
		float toAvg = 0.0;
		for (int j = 0; j < bucketHistory.size(); j++) {
			toAvg += bucketHistory[j].at(i);
		}
		buckets[i] = toAvg / bucketHistory.size();
	}
}

void fixBuckets(double a, int id) {
	if (id < 100) {
		bucketDirty[0] += a;
		return;
	}
	if (id < 250) {
		bucketDirty[1] += a;
		return;
	}
	if (id < 500) {
		bucketDirty[2] += a;
		return;
	}
	if (id < 700) {
		bucketDirty[3] += a;
		return;
	}
	if (id < 900) {
		bucketDirty[4] += a;
		return;
	}
	if (id < 1400) {
		bucketDirty[5] += a;
		return;
	}
	if (id < 2000) {
		bucketDirty[6] += a;
	}
}