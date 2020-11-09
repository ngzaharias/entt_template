#include "Application.h"
#include "GameApplication.h"
#include "Types.h"

#include <direct.h>

Application* application = nullptr;

int main(int agrc, char* argv[])
{
	_chdir("../../");

	application = new GameApplication();
	application->Execute(agrc, argv);
	delete(application);

	application = nullptr;

	return 0;
}