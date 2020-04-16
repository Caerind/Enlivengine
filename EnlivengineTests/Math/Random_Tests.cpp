#include <Enlivengine/Math/Random.hpp>

#include <doctest/doctest.h>

DOCTEST_TEST_CASE("Random")
{
	en::I32 min = 1;
	en::I32 max = 100000000;
	en::I32 times = 10;

	en::RandomEngine random1(12345);
	en::RandomEngine random2(345678376);
	en::RandomEngine random3(12345);

	en::I32 p1 = 0;
	en::I32 p2 = 0;
	en::I32 p3 = 0;
	en::I32 p4 = 0;

	for (en::I32 i = 0; i < times; i++)
	{
		en::I32 n1 = random1.get(min, max);
		en::I32 n2 = random2.get(min, max);
		en::I32 n3 = random3.get(min, max);
		en::I32 n4 = en::Random::get<en::I32>(min, max);

		DOCTEST_CHECK(n1 == n3);
		DOCTEST_CHECK(n1 != n2);
		DOCTEST_CHECK(n1 != n4);

		DOCTEST_CHECK(n1 != p1);
		DOCTEST_CHECK(n2 != p2);
		DOCTEST_CHECK(n3 != p3);
		DOCTEST_CHECK(n4 != p4);

		p1 = n1;
		p2 = n2;
		p3 = n3;
		p4 = n4;
	}
}