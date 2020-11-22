#include "Editor/Application.h"

#include <direct.h>

int main(int agrc, char* argv[])
{
	_chdir("../../");

	editor::Application* application = new editor::Application();
	application->Execute(agrc, argv);
	delete(application);

	return 0;
}