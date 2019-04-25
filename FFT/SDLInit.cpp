#include "SDL.h"
#include <glew.h>
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
double buckets[7];


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

	setOpenGLAttributes();
	SDL_GL_MakeCurrent(window, mainContext);

	//Initialize GLEW
	glewExperimental = GL_TRUE;
	GLenum glewError = glewInit();
	if (glewError != GLEW_OK) {
		std::cout << "Error initializing GLEW! %s\n" << glewGetErrorString(glewError) << std::endl;

	}

	const GLubyte* renderer = glGetString(GL_RENDERER);  // get renderer string
	const GLubyte* version = glGetString(GL_VERSION);    // version as a string
	std::cout << "Renderer: " << renderer << "\n";
	std::cout << "OpenGL version supported:" << version << std::endl;
	//V-sync
	SDL_GL_SetSwapInterval(1);

	Mix_OpenAudio(22050, AUDIO_S16SYS, 2, 1024);
	sound = Mix_LoadWAV(BOOM_BOOM);
	if (sound == NULL) {
		std::cout << "Something Broke" << std::endl;
		return 0;
	}

	int channel = Mix_PlayChannel(0, sound, -1);
	Mix_RegisterEffect(MIX_CHANNEL_POST, passThrough, NULL, NULL);

	// Capture and hold the mouse position
	SDL_SetRelativeMouseMode(SDL_TRUE);

	return true;
}

bool setOpenGLAttributes() {
	//Set the openGL version
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);

	//Double Buffering
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

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

	//Reset the buckets on every run
	for (int i = 0; i < 7; i++) {
		buckets[i] = 0.0;
	}

	/* Iterate over p 2 by 2 */
	for (int i = 0; i < length / 2; i++) {
		FFTdata[i] = (std::complex<double>)(short)(p[i * 2] * value);
		p[i * 2] = (short)(p[i * 2] * value);
		p[i * 2 + 1] = (short)(p[i * 2 + 1] * value);
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
	buckets[0] = buckets[0] / 100;
	buckets[1] = buckets[1] / 150;
	buckets[2] = buckets[2] / 250;
	buckets[3] = buckets[3] / 200;
	buckets[4] = buckets[4] / 200;
	buckets[5] = buckets[5] / 500;
	buckets[6] = buckets[6] / 600;
}

void fixBuckets(double a, int id) {
	if (id < 100) {
		buckets[0] += a;
		return;
	}
	if (id < 250) {
		buckets[1] += a;
		return;
	}
	if (id < 500) {
		buckets[2] += a;
		return;
	}
	if (id < 700) {
		buckets[3] += a;
		return;
	}
	if (id < 900) {
		buckets[4] += a;
		return;
	}
	if (id < 1400) {
		buckets[5] += a;
		return;
	}
	if (id < 2000) {
		buckets[6] += a;
	}
}