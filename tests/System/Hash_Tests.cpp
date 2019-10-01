#include <Enlivengine/System/Hash.hpp>

#include <doctest/doctest.h>

DOCTEST_TEST_CASE("Conversion between U32 / F32")
{
	en::U32 a = en::Hash::CRC32("A");
	en::U32 b = en::Hash::CRC32("B");
	en::U32 c = en::Hash::CRC32("AZERTYUIOPQSDFGHJKLSWXCVGBHJGFCVBNJKIUYGHJDKIQUYGDHNJQKIDUYHGBN1234567890,;:!?.?./");
	en::U32 d = en::Hash::CRC32("A");

	DOCTEST_CHECK(a != 0);
	DOCTEST_CHECK(b != 0);
	DOCTEST_CHECK(c != 0);
	DOCTEST_CHECK(a != b);
	DOCTEST_CHECK(a != c);
	DOCTEST_CHECK(b != c);
	DOCTEST_CHECK(a == d);
}