#include <Enlivengine/Math/Utilities.hpp>

#include <doctest/doctest.h>

DOCTEST_TEST_CASE("Utilities")
{
	DOCTEST_CHECK(en::Math::Floor(2.0f) == 2);
	DOCTEST_CHECK(en::Math::Ceil(2.0f) == 2);
	DOCTEST_CHECK(en::Math::Floor(2.4f) == 2);
	DOCTEST_CHECK(en::Math::Ceil(2.4f) == 3);
	DOCTEST_CHECK(en::Math::Floor(-2.7f) == -3);
	DOCTEST_CHECK(en::Math::Ceil(-2.7f) == -2);
	DOCTEST_CHECK(en::Math::Floor(-2.0f) == -2);
	DOCTEST_CHECK(en::Math::Ceil(-2.0f) == -2);
	DOCTEST_CHECK(en::Math::Clamp(50, 0, 10) == 10);
	DOCTEST_CHECK(en::Math::Clamp(50, 0, 100) == 50);
	DOCTEST_CHECK(en::Math::Clamp(50, 100, 1000) == 100);
	DOCTEST_CHECK(en::Math::InRange(50, 0, 100));
	DOCTEST_CHECK(en::Math::Equals(50.0000f, 50.0001f, 0.001f));

	DOCTEST_CHECK(en::Math::Equals(en::Math::Cos(90.0f), 0.0f));
	DOCTEST_CHECK(en::Math::Equals(en::Math::Cos(0.0f), 1.0f));
	DOCTEST_CHECK(en::Math::Equals(en::Math::Sin(90.0f), 1.0f));
	DOCTEST_CHECK(en::Math::Equals(en::Math::Sin(0.0f), 0.0f));
	DOCTEST_CHECK(en::Math::Equals(en::Math::Sqrt(1.0f), 1.0f));
	DOCTEST_CHECK(en::Math::Equals(en::Math::Sqrt(4.0f), 2.0f));
	DOCTEST_CHECK(en::Math::Equals(en::Math::Sqrt(9.0f), 3.0f));
	DOCTEST_CHECK(en::Math::Equals(en::Math::Sqrt(16.0f), 4.0f));

	// TODO : Improve tests
}