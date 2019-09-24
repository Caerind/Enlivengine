// We need to define this before including the .hpp file as doctest include is in the .hpp
#define DOCTEST_CONFIG_IMPLEMENT
#include "testrunner.hpp"

namespace en
{

int TestRunner::RunTests(int argc, char** argv)
{
	doctest::Context context;

	// defaults
	//context.addFilter("test-case-exclude", "*math*"); // exclude test cases with "math" in their name
	//context.setOption("abort-after", 5);              // stop test execution after 5 failed assertions
	//context.setOption("order-by", "name");            // sort the test cases by their name

	context.applyCommandLine(argc, argv);

	// overrides
	//context.setOption("no-breaks", true);             // don't break in the debugger when assertions fail

	int res = context.run();

	if (context.shouldExit())
		return k_ShouldExitCode;

	return res;
}

} // namespace en