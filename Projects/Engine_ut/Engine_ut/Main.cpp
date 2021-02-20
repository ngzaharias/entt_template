//#define CATCH_CONFIG_MAIN

#define CATCH_CONFIG_RUNNER
#include <Catch2/catch.hpp>

int main(int argc, char* argv[]) 
{
	// global setup...

	int result = Catch::Session().run(argc, argv);

	// global clean-up...

	system("pause");
	return result;
}	