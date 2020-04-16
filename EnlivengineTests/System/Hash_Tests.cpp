#include <Enlivengine/System/Hash.hpp>

#include <doctest/doctest.h>

DOCTEST_TEST_CASE("HASH CRC32 - char*")
{
    const en::U32 a = en::Hash::CRC32("A");
    const en::U32 b = en::Hash::CRC32("B");
    const en::U32 c = en::Hash::CRC32("AZERTYUIOPQSDFGHJKLSWXCVGBHJGFCVBNJKIUYGHJDKIQUYGDHNJQKIDUYHGBN1234567890,;:!?.?./");
    const en::U32 d = en::Hash::CRC32("A");
    const en::U32 e = en::Hash::CRC32("A ");

    DOCTEST_CHECK(a != 0);
    DOCTEST_CHECK(b != 0);
    DOCTEST_CHECK(c != 0);
    DOCTEST_CHECK(a != b);
    DOCTEST_CHECK(a != c);
    DOCTEST_CHECK(b != c);
    DOCTEST_CHECK(a == d);
    DOCTEST_CHECK(a != e);
}

DOCTEST_TEST_CASE("HASH CRC32 - string_view")
{
    const en::U32 a = en::Hash::CRC32(std::string_view("A"));
    const en::U32 b = en::Hash::CRC32(std::string_view("B"));
    const en::U32 c = en::Hash::CRC32(std::string_view("AZERTYUIOPQSDFGHJKLSWXCVGBHJGFCVBNJKIUYGHJDKIQUYGDHNJQKIDUYHGBN1234567890,;:!?.?./"));
    const en::U32 d = en::Hash::CRC32(std::string_view("A"));
    const en::U32 e = en::Hash::CRC32(std::string_view("A "));

    DOCTEST_CHECK(a != 0);
    DOCTEST_CHECK(b != 0);
    DOCTEST_CHECK(c != 0);
    DOCTEST_CHECK(a != b);
    DOCTEST_CHECK(a != c);
    DOCTEST_CHECK(b != c);
    DOCTEST_CHECK(a == d);
    DOCTEST_CHECK(a != e);
}

DOCTEST_TEST_CASE("HASH CRC32 - char* == string_view")
{
    const en::U32 a = en::Hash::CRC32("A");
    const en::U32 b = en::Hash::CRC32("B");
    const en::U32 c = en::Hash::CRC32("AZERTYUIOPQSDFGHJKLSWXCVGBHJGFCVBNJKIUYGHJDKIQUYGDHNJQKIDUYHGBN1234567890,;:!?.?./");
    const en::U32 d = en::Hash::CRC32("A");
    const en::U32 e = en::Hash::CRC32("A ");

    const en::U32 asv = en::Hash::CRC32(std::string_view("A"));
    const en::U32 bsv = en::Hash::CRC32(std::string_view("B"));
    const en::U32 csv = en::Hash::CRC32(std::string_view("AZERTYUIOPQSDFGHJKLSWXCVGBHJGFCVBNJKIUYGHJDKIQUYGDHNJQKIDUYHGBN1234567890,;:!?.?./"));
    const en::U32 dsv = en::Hash::CRC32(std::string_view("A"));
    const en::U32 esv = en::Hash::CRC32(std::string_view("A "));

    DOCTEST_CHECK(a == asv);
    DOCTEST_CHECK(b == bsv);
    DOCTEST_CHECK(c == csv);
    DOCTEST_CHECK(d == dsv);
    DOCTEST_CHECK(e == esv);
}