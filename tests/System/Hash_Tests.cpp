#include <Enlivengine/System/Hash.hpp>

#include <doctest/doctest.h>

DOCTEST_TEST_CASE("Conversion between U32 / F32")
{
	en::U32 a = en::Hash::Meow("A");
	en::U32 b = en::Hash::Meow("B");
	en::U32 c = en::Hash::Meow("AZERTYUIOPQSDFGHJKLSWXCVGBHJGFCVBNJKIUYGHJDKIQUYGDHNJQKIDUYHGBN1234567890,;:!?.?./");

	DOCTEST_CHECK(a != 0);
	DOCTEST_CHECK(b != 0);
	DOCTEST_CHECK(c != 0);
	DOCTEST_CHECK(a != b);
	DOCTEST_CHECK(a != c);
	DOCTEST_CHECK(b != c);
}