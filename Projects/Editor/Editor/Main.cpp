#include "Editor/EditorPCH.h"
#include "Editor/Application.h"

#include <direct.h>

int main(int agrc, char* argv[])
{
	// #todo: we shouldn't need to change directory
	_chdir("../../");

	editor::Application* application = new editor::Application();
	application->Execute(agrc, argv);
	delete(application);

	return 0;
}