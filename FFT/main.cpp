#include "discord.cpp"
#include "SDL.h"
#include "SDL_mixer.h"
#include "FFT.h"
#include <iostream>
#include <string>
#include <GL/glew.h>
#include <stdio.h>
#include "SDL_opengl.h"


static const char* BOOM_BOOM = "C:\\Users\\21aro\\Music\\800+440.wav";

double buckets[7];

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
	if (id < 2000)
	{
		buckets[6] += a;
	}
}

// make a passthru processor function that does nothing...
void passThrough(int chan, void* stream, int len, void* udata)
{
	float value = 1.0f;
	short* p = (short*)stream;
	int length = len / 2;

	/* Iterate over p 2 by 2 */
	for (int i = 0; i < length / 2; i++)
	{
		FFTdata[i] = (std::complex<double>)(short)(p[i * 2] * value);
		p[i * 2] = (short)(p[i * 2] * value);
		p[i * 2 + 1] = (short)(p[i * 2 + 1] * value);
	}

	CArray data(FFTdata, 2048);
	fft(data);
	double amplitude = -1000;
	int maxI = -1;
	// Why -512? Because that's around 6kHz and who cares about screeches
	for (int i = 0; i < data.size() - 256; i++)
	{
		fixBuckets(std::sqrt(data[i].real() * data[i].real() + data[i].imag() * data[i].imag()), (i * 22050 / 2) / (2048 / 2));
	}
	normalizeBuckets();
	for (int i = 0; i < 7; i++)
	{
		std::cout << buckets[i] << " ";
		buckets[i] = 0.0;
	}
	std::cout << std::endl;
}


int main(int argc, char* argv[])
{
	discordInit();
	updateDiscordPresence();

	int result = 0;

	if (SDL_Init(SDL_INIT_AUDIO) < 0) {
	}
	Mix_OpenAudio(22050, AUDIO_S16SYS, 2, 1024);
	Mix_Chunk* sound = Mix_LoadWAV(BOOM_BOOM);
	if (sound == NULL)
	{
		std::cout << "Something Broke" << std::endl;
		return 0;
	}

	int channel = Mix_PlayChannel(0, sound, -1);
	Mix_RegisterEffect(MIX_CHANNEL_POST, passThrough, NULL, NULL);

	while (!SDL_QuitRequested()) {
		SDL_Delay(16);
		
	}

	Mix_FreeChunk(sound);
	Mix_CloseAudio();
	SDL_Quit();
	Discord_Shutdown();
	return 0;
}