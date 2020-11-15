#include "Game/Application.h"

#include <direct.h>

int main(int agrc, char* argv[])
{
	_chdir("../../");

	game::Application* application = new game::Application();
	application->Execute(agrc, argv);
	delete(application);

	return 0;
}