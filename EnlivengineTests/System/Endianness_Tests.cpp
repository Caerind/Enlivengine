#include <Enlivengine/System/Endianness.hpp>

#include <doctest/doctest.h>

DOCTEST_TEST_CASE("Endianness swapping")
{
	DOCTEST_SUBCASE("Bytes")
	{
		en::U32 t = 123;
		en::EndianSwapBytes(nullptr, 0);
		en::EndianSwapBytes(nullptr, 1);
		en::EndianSwapBytes(&t, 0);
		DOCTEST_CHECK(t == 123);
					
		en::U8 buf1[1] = { 0xAB };
		en::EndianSwapBytes(buf1, 1);
		DOCTEST_CHECK(buf1[0] == 0xAB);

		en::U8 buf2[2] = { 0xAB, 0xCD };
		en::EndianSwapBytes(buf2, 2);
		DOCTEST_CHECK(buf2[0] == 0xCD);
		DOCTEST_CHECK(buf2[1] == 0xAB);
		en::EndianSwapBytes(buf2, 2);
		DOCTEST_CHECK(buf2[0] == 0xAB);
		DOCTEST_CHECK(buf2[1] == 0xCD);

		en::U8 buf3[3] = { 0xAB, 0xCD, 0xEF };
		en::EndianSwapBytes(buf3, 3);
		DOCTEST_CHECK(buf3[0] == 0xEF);
		DOCTEST_CHECK(buf3[1] == 0xCD);
		DOCTEST_CHECK(buf3[2] == 0xAB);
		en::EndianSwapBytes(buf3, 3);
		DOCTEST_CHECK(buf3[0] == 0xAB);
		DOCTEST_CHECK(buf3[1] == 0xCD);
		DOCTEST_CHECK(buf3[2] == 0xEF);

		en::U8 buf4[4] = { 0xAB, 0xCD, 0x12, 0x34 };
		en::EndianSwapBytes(buf4, 4);
		DOCTEST_CHECK(buf4[0] == 0x34);
		DOCTEST_CHECK(buf4[1] == 0x12);
		DOCTEST_CHECK(buf4[2] == 0xCD);
		DOCTEST_CHECK(buf4[3] == 0xAB);
		en::EndianSwapBytes(buf4, 4);
		DOCTEST_CHECK(buf4[0] == 0xAB);
		DOCTEST_CHECK(buf4[1] == 0xCD);
		DOCTEST_CHECK(buf4[2] == 0x12);
		DOCTEST_CHECK(buf4[3] == 0x34);
	}

	DOCTEST_SUBCASE("U16 & I16")
	{
		DOCTEST_CHECK(en::EndianSwapU16(0xABCD) == 0xCDAB);
		DOCTEST_CHECK(en::EndianSwapI16(0x1234) == 0x3412);

		DOCTEST_CHECK(en::EndianSwapU16(en::EndianSwapU16(0xABCD)) == 0xABCD);
		DOCTEST_CHECK(en::EndianSwapI16(en::EndianSwapI16(0x1234)) == 0x1234);
	}

	DOCTEST_SUBCASE("U32 & I32")
	{
		DOCTEST_CHECK(en::EndianSwapU32(0xABCD1234) == 0x3412CDAB);
		DOCTEST_CHECK(en::EndianSwapI32(0xABCD1234) == 0x3412CDAB);

		DOCTEST_CHECK(en::EndianSwapU32(en::EndianSwapU32(0xABCD1234)) == 0xABCD1234);
		DOCTEST_CHECK(en::EndianSwapI32(en::EndianSwapI32(0xABCD1234)) == 0xABCD1234);
	}

	DOCTEST_SUBCASE("F32")
	{
		// TODO : Test this
	}
}