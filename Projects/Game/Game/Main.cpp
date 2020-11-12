#define _CRT_FUNCTIONS_REQUIRED 1

#include "Game/GameApplication.h"

#include <direct.h>

GameApplication* application = nullptr;

int main(int agrc, char* argv[])
{
	_chdir("../../");

	application = new GameApplication();
	application->Execute(agrc, argv);
	delete(application);

	application = nullptr;

	return 0;
}