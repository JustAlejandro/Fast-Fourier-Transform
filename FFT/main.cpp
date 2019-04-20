#include "discord.cpp"
#include <iostream>
#include <string>
using std::string;

int main(int argc, char* argv[])
{
	discordInit();
	updateDiscordPresence();
	string input = "";
	while (input != "stop")
	{
		std::cin >> input;
	}

	Discord_Shutdown();
	return 0;
}