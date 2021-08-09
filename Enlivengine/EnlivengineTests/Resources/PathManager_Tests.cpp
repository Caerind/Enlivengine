#include <Enlivengine/Resources/PathManager.hpp>

#include <doctest/doctest.h>

DOCTEST_TEST_CASE("PathManager")
{
	// TODO : Find how to test
	// Path management is really boring and very error prone
	// We should test this automatically
	// 
	// The only tests I can think of right now is 
	// - assert that all paths ends with /
	// - assert that all paths don't contains \
	// - assert that all paths are absolute
	// 
	// But the thing is that all path depends on the setup
	// Paths are given via IDE/build folder/"runtime" folder
}