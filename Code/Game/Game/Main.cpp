#include "GamePCH.h"
#include "Game/Application.h"

#include <direct.h>

int main(int agrc, char* argv[])
{
	// #todo: we shouldn't need to change directory
	// #fixme: this won't work when running the game via the exe (only the solution)
	_chdir("../../");

	game::Application* application = new game::Application();
	application->Execute(agrc, argv);
	delete(application);

	return 0;
}