#include "Editor/Application.h"

#include "Editor/Experimental/experimental_containers.h"
#include "Editor/Experimental/experimental_pod.h"
#include "Editor/Experimental/experimental_variant.h"

#include <direct.h>

int main(int agrc, char* argv[])
{
	// #todo: we shouldn't need to change directory
	_chdir("../../");

	//experimental_containers();
	//experimental_pod();
	//experimental_variant();

	editor::Application* application = new editor::Application();
	application->Execute(agrc, argv);
	delete(application);

	return 0;
}