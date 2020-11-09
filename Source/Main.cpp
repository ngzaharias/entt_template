#include "Strings/Name.hpp"
#include "Strings/NameTable.hpp"

#include "Application.hpp"
#include "GameApplication.hpp"
#include "Types.hpp"

#include <direct.h>

Application* application = nullptr;
string::NameTable* nameTable = nullptr;

int main(int agrc, char* argv[])
{
	_chdir("../../");

	application = new GameApplication();
	application->Execute(agrc, argv);
	delete(application);

	application = nullptr;

	return 0;
}