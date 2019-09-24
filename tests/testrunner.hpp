#pragma once

// This force to add DOCTEST_ before each doctest macro, which is "boring" but much safer
#define DOCTEST_CONFIG_NO_SHORT_MACRO_NAMES 

// This could be enabled to debug strings
//#define DOCTEST_CONFIG_TREAT_CHAR_STAR_AS_STRING

// This could be enabled on Release
//#define DOCTEST_CONFIG_SUPER_FAST_ASSERTS

// Doctest is using some forward declaration. 
// In case, we encounter a problem (which might not happen as it is heavily used/tested), we should enable this
//#define DOCTEST_CONFIG_USE_STD_HEADERS

#include "../thirdparty/doctest/doctest.h"

namespace en
{

class TestRunner
{
public:
	TestRunner() = delete;

	static int RunTests(int argc, char** argv);

	static constexpr int k_ShouldExitCode = -1000;
};

} // namespace en