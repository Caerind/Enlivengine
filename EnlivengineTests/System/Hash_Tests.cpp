#include <Enlivengine/System/Hash.hpp>

#include <doctest/doctest.h>

#include <string_view>

DOCTEST_TEST_CASE("Constexpr Hash")
{
	DOCTEST_SUBCASE("std::string_view == const char*")
	{
		std::string_view azertysv = "AZERTYUIOPQSDFGHJKLSWXCVGBHJGFCVBNJKIUYGHJDKIQUYGDHNJQKIDUYHGBN1234567890,;:!?.?./1234567890098765432123456789098765432123456789098765432134567892345YTFBHJTRDFGHREDFG891Y1GUIAHZ TZGIUIZJ19YT7UFAJHVBSIASHUSHGC VNZIU HIUG IQHDU QDGYTQGHJBQHUIIGQDIHBJNQKS";
		std::string_view asv = "A";
		std::string_view bsv = "B";
		std::string_view a_sv = "A "; 
		const char* azertyc = "AZERTYUIOPQSDFGHJKLSWXCVGBHJGFCVBNJKIUYGHJDKIQUYGDHNJQKIDUYHGBN1234567890,;:!?.?./1234567890098765432123456789098765432123456789098765432134567892345YTFBHJTRDFGHREDFG891Y1GUIAHZ TZGIUIZJ19YT7UFAJHVBSIASHUSHGC VNZIU HIUG IQHDU QDGYTQGHJBQHUIIGQDIHBJNQKS";
		const char* ac = "A";
		const char* bc = "B";
		const char* a_c = "A ";

		en::U32 a1 = en::Hash::SlowHash(azertysv);
		en::U32 a2 = en::Hash::SlowHash(azertyc);
		DOCTEST_CHECK(a1 == a2);

		a1 = en::Hash::SlowHash(asv);
		a2 = en::Hash::SlowHash(ac);
		DOCTEST_CHECK(a1 == a2);

		a1 = en::Hash::SlowHash(bsv);
		a2 = en::Hash::SlowHash(bc);
		DOCTEST_CHECK(a1 == a2);

		a1 = en::Hash::SlowHash(a_sv);
		a2 = en::Hash::SlowHash(a_c);
		DOCTEST_CHECK(a1 == a2);
	}
}

DOCTEST_TEST_CASE("Combine hash")
{
	DOCTEST_SUBCASE("SlowHash")
	{
		const en::U32 a = en::Hash::SlowHash("A");
		const en::U32 b = en::Hash::SlowHash("B");
		const en::U32 c = en::Hash::SlowHash("C");
		const en::U32 caa = en::Hash::Combine32(a, a);
		const en::U32 cab = en::Hash::Combine32(a, b);
		const en::U32 cba = en::Hash::Combine32(b, a);
		const en::U32 cbb = en::Hash::Combine32(b, b);

		DOCTEST_CHECK(caa != cab);
		DOCTEST_CHECK(caa != cba);
		DOCTEST_CHECK(caa != cbb);
		DOCTEST_CHECK(cab != cba);
		DOCTEST_CHECK(cab != cbb);
		DOCTEST_CHECK(cba != cbb);
	}
}
