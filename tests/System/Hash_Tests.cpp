#include <Enlivengine/System/Hash.hpp>

#include <doctest/doctest.h>

DOCTEST_TEST_CASE("Conversion between U32 / F32")
{
	DOCTEST_CHECK(en::Hash::CRC32("Test") == -1493765679);
}