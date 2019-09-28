#include <Enlivengine/Math/Random.hpp>

#include <doctest/doctest.h>

DOCTEST_TEST_CASE("Random")
{
	en::I32 min = 0;
	en::I32 max = 100000000;
	en::I32 times = 10;

	en::RandomEngine random1(12345);
	en::RandomEngine random2(345678376);
	en::RandomEngine random3(12345);

	for (en::I32 i = 0; i < times; i++)
	{
		en::I32 n1 = random1.get(min, max);
		en::I32 n2 = random2.get(min, max);
		en::I32 n3 = random3.get(min, max);
		en::I32 n4 = en::Random::get(min, max);

		DOCTEST_CHECK(n1 == n3);
		DOCTEST_CHECK(n1 != n2);
		DOCTEST_CHECK(n1 != n4);
	}

	// TODO : Improve tests ?
}