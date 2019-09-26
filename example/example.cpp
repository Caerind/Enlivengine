#include <Enlivengine/System/Macros.hpp>
#include <Enlivengine/System/PlatformDetection.hpp>
#include <Enlivengine/System/CompilerDetection.hpp>
#include <Enlivengine/System/CompilerTraits.hpp>
#include <Enlivengine/System/ByteUnits.hpp>
#include <Enlivengine/System/PrimitiveTypes.hpp>
#include <Enlivengine/System/Endianness.hpp>
#include <Enlivengine/System/Debugger.hpp>

// TODO : Remove
#include <iostream>
#include <cstdlib>
#include <cstdio>

int main(int argc, char** argv)
{
	std::cout << "ENLIVE_FUNCTION : " << ENLIVE_FUNCTION << std::endl;
	std::cout << "ENLIVE_COMPILER_NAME : " << ENLIVE_COMPILER_NAME << std::endl;
	std::cout << "ENLIVE_COMPILER_VERSION : " << ENLIVE_COMPILER_VERSION << std::endl;
	std::cout << "ENLIVE_COMPILER_STRING : " << ENLIVE_COMPILER_STRING << std::endl;
	std::cout << "ENLIVE_PLATFORM_NAME : " << ENLIVE_PLATFORM_NAME << std::endl;
	std::cout << "ENLIVE_PLATFORM_DESCRIPTION : " << ENLIVE_PLATFORM_DESCRIPTION << std::endl;

	//en::Debugger::Break();

	return 0;
}
