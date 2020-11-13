#define _CRT_FUNCTIONS_REQUIRED 1

#include "Game/Application.h"

#include <direct.h>

game::Application* application = nullptr;

int main(int agrc, char* argv[])
{
	_chdir("../../");

	application = new game::Application();
	application->Execute(agrc, argv);
	delete(application);

	application = nullptr;

	return 0;
}