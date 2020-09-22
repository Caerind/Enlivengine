#include <Enlivengine/System/PrimitiveTypes.hpp>

#include <doctest/doctest.h>

DOCTEST_TEST_CASE("Testing primitive types size")
{
	DOCTEST_CHECK(sizeof(en::I8) == 1);
	DOCTEST_CHECK(sizeof(en::U8) == 1);
	DOCTEST_CHECK(sizeof(en::I16) == 2);
	DOCTEST_CHECK(sizeof(en::U16) == 2);
	DOCTEST_CHECK(sizeof(en::I32) == 4);
	DOCTEST_CHECK(sizeof(en::U32) == 4);
	DOCTEST_CHECK(sizeof(en::I64) == 8);
	DOCTEST_CHECK(sizeof(en::U64) == 8);

	DOCTEST_CHECK(sizeof(en::F32) == 4);
	DOCTEST_CHECK(sizeof(en::F64) == 8);
#if defined(ENLIVE_DOUBLE_PRECISION)
	DOCTEST_CHECK(sizeof(en::Real) == 8);
#else
	DOCTEST_CHECK(sizeof(en::Real) == 4);
#endif
}

DOCTEST_TEST_CASE("Testing primitive types min/max")
{
	DOCTEST_SUBCASE("Signed integers")
	{
		// Values directly
		/*
		DOCTEST_CHECK(en::I8_Min == -128);
		DOCTEST_CHECK(en::I8_Max == 127);
		DOCTEST_CHECK(en::I16_Min == -32768);
		DOCTEST_CHECK(en::I16_Max == 32767);
		DOCTEST_CHECK(en::I32_Min == -2147483648i32);
		DOCTEST_CHECK(en::I32_Max == 2147483647i32);
		DOCTEST_CHECK(en::I64_Min == -9223372036854775808i64);
		DOCTEST_CHECK(en::I64_Max == 9223372036854775807i64);
		*/


		// Correct type limits
		#if defined(ENLIVE_COMPILER_MSVC)
			#pragma warning(disable: 4307)
			#pragma warning(disable: 4310)
            #pragma warning(disable: 26450)
		#elif defined(ENLIVE_COMPILER_GNUC)
			#pragma GCC diagnostic push
			#pragma GCC diagnostic ignored "-Woverflow"
		#endif

		DOCTEST_CHECK(en::I8(en::I8_Max + 1) == en::I8_Min);
		DOCTEST_CHECK(en::I8(en::I8_Min - 1) == en::I8_Max);
		DOCTEST_CHECK(en::I16(en::I16_Max + 1) == en::I16_Min);
		DOCTEST_CHECK(en::I16(en::I16_Min - 1) == en::I16_Max);
		DOCTEST_CHECK(en::I32(en::I32_Max + 1) == en::I32_Min);
		DOCTEST_CHECK(en::I32(en::I32_Min - 1) == en::I32_Max);
		DOCTEST_CHECK(en::I64(en::I64_Max + 1) == en::I64_Min);
		DOCTEST_CHECK(en::I64(en::I64_Min - 1) == en::I64_Max);

		#if defined(ENLIVE_COMPILER_MSVC)
			#pragma warning(default: 4307)
			#pragma warning(default: 4310)
            #pragma warning(default: 26450)
		#elif defined(ENLIVE_COMPILER_GNUC)
			#pragma GCC diagnostic pop
		#endif
	}

	DOCTEST_SUBCASE("Unsigned integers")
	{
		// Values directly
		/*
		DOCTEST_CHECK(en::U8_Min == 0);
		DOCTEST_CHECK(en::U8_Max == 255);
		DOCTEST_CHECK(en::U16_Min == 0);
		DOCTEST_CHECK(en::U16_Max == 65535);
		DOCTEST_CHECK(en::U32_Min == 0);
		DOCTEST_CHECK(en::U32_Max == 4294967295);
		DOCTEST_CHECK(en::U64_Min == 0);
		DOCTEST_CHECK(en::U64_Max == 18446744073709551615);
		*/


		// Correct type limits
		#if defined(ENLIVE_COMPILER_MSVC)
			#pragma warning(disable: 4307)
            #pragma warning(disable: 26450)
		#elif defined(ENLIVE_COMPILER_GNUC)
			#pragma GCC diagnostic push
			#pragma GCC diagnostic ignored "-Woverflow"
		#endif

		DOCTEST_CHECK(en::U8_Max == en::U8(-1));
		DOCTEST_CHECK(en::U16_Max == en::U16(-1));
		DOCTEST_CHECK(en::U32_Max == en::U32(-1));
		DOCTEST_CHECK(en::U64_Max == en::U64(-1)); 

		#if defined(ENLIVE_COMPILER_MSVC)
			#pragma warning(default: 4307)
            #pragma warning(default: 26450)
		#elif defined(ENLIVE_COMPILER_GNUC)
			#pragma GCC diagnostic pop
		#endif
	}
}