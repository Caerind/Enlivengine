#include "tests/testrunner.hpp"

#include "src/system/macros.hpp"
#include "src/system/platformdetection.hpp"
#include "src/system/compilerdetection.hpp"
#include "src/system/compilertraits.hpp"
#include "src/system/primitivetypes.hpp"
#include "src/system/byteunits.hpp"

// TODO : Remove
#include <iostream>
#include <cstdlib>
#include <cstdio>

int main(int argc, char** argv)
{
	int testResult = en::TestRunner::RunTests(argc, argv);
	if (testResult == en::TestRunner::k_ShouldExitCode)
	{
		return 0;
	}

	std::cout << "ENLIVE_FUNCTION : " << ENLIVE_FUNCTION << std::endl;
	std::cout << "ENLIVE_COMPILER_NAME : " << ENLIVE_COMPILER_NAME << std::endl;
	std::cout << "ENLIVE_COMPILER_VERSION : " << ENLIVE_COMPILER_VERSION << std::endl;
	std::cout << "ENLIVE_COMPILER_STRING : " << ENLIVE_COMPILER_STRING << std::endl;
	std::cout << "ENLIVE_PLATFORM_NAME : " << ENLIVE_PLATFORM_NAME << std::endl;
	std::cout << "ENLIVE_PLATFORM_DESCRIPTION : " << ENLIVE_PLATFORM_DESCRIPTION << std::endl;

	return 0;
}