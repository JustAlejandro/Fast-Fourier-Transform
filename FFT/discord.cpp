#define _CRT_SECURE_NO_WARNINGS /* thanks Microsoft */

#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <cstdio>
#include <chrono>
#include <ctime>

#include "discord_rpc.h"

static const char* APPLICATION_ID = "568953277266395136";
static int64_t StartTime;// = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
static int SendPresence = 1;

static void updateDiscordPresence()
{
	if (SendPresence) {
		char buffer[256];
		DiscordRichPresence discordPresence;
		memset(&discordPresence, 0, sizeof(discordPresence));
		discordPresence.state = "Convolving Signals";
		sprintf(buffer, "In Visual Studio");
		discordPresence.details = buffer;
		discordPresence.startTimestamp = StartTime;
		discordPresence.endTimestamp = NULL;
		discordPresence.largeImageKey = "smallimage";
		discordPresence.smallImageKey = "largeimage";
		discordPresence.instance = 0;
		Discord_UpdatePresence(&discordPresence);
	}
	else {
		Discord_ClearPresence();
	}
}

static void discordInit()
{
	StartTime = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
	DiscordEventHandlers handlers;
	memset(&handlers, 0, sizeof(handlers));
	Discord_Initialize(APPLICATION_ID, &handlers, 1, NULL);
}

