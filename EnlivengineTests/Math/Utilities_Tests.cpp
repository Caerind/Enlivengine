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

	DOCTEST_CHECK(en::Math::Equals(en::Math::Cos(0.0f), 1.0f));
	DOCTEST_CHECK(en::Math::Equals(en::Math::Cos(15.0f), 0.9659f));
	DOCTEST_CHECK(en::Math::Equals(en::Math::Cos(22.5f), 0.9238f));
	DOCTEST_CHECK(en::Math::Equals(en::Math::Cos(30.0f), 0.8660f));
	DOCTEST_CHECK(en::Math::Equals(en::Math::Cos(45.0f), 0.7071f));
	DOCTEST_CHECK(en::Math::Equals(en::Math::Cos(60.0f), 0.5f));
	DOCTEST_CHECK(en::Math::Equals(en::Math::Cos(67.5f), 0.3826f));
	DOCTEST_CHECK(en::Math::Equals(en::Math::Cos(75.0f), 0.2588f));
	DOCTEST_CHECK(en::Math::Equals(en::Math::Cos(90.0f), 0.0f));
	DOCTEST_CHECK(en::Math::Equals(en::Math::Cos(120.0f), -0.5f));
	DOCTEST_CHECK(en::Math::Equals(en::Math::Cos(135.0f), -0.7071f));
	DOCTEST_CHECK(en::Math::Equals(en::Math::Cos(150.0f), -0.8660f));
	DOCTEST_CHECK(en::Math::Equals(en::Math::Cos(180.0f), -1.0f));
	DOCTEST_CHECK(en::Math::Equals(en::Math::Cos(-0.0f), 1.0f));
	DOCTEST_CHECK(en::Math::Equals(en::Math::Cos(-15.0f), 0.9659f));
	DOCTEST_CHECK(en::Math::Equals(en::Math::Cos(-22.5f), 0.9238f));
	DOCTEST_CHECK(en::Math::Equals(en::Math::Cos(-30.0f), 0.8660f));
	DOCTEST_CHECK(en::Math::Equals(en::Math::Cos(-45.0f), 0.7071f));
	DOCTEST_CHECK(en::Math::Equals(en::Math::Cos(-60.0f), 0.5f));
	DOCTEST_CHECK(en::Math::Equals(en::Math::Cos(-67.5f), 0.3826f));
	DOCTEST_CHECK(en::Math::Equals(en::Math::Cos(-75.0f), 0.2588f));
	DOCTEST_CHECK(en::Math::Equals(en::Math::Cos(-90.0f), 0.0f));
	DOCTEST_CHECK(en::Math::Equals(en::Math::Cos(-120.0f), -0.5f));
	DOCTEST_CHECK(en::Math::Equals(en::Math::Cos(-135.0f), -0.7071f));
	DOCTEST_CHECK(en::Math::Equals(en::Math::Cos(-150.0f), -0.8660f));
	DOCTEST_CHECK(en::Math::Equals(en::Math::Cos(-180.0f), -1.0f));

	DOCTEST_CHECK(en::Math::Equals(en::Math::Sin(0.0f), 0.0f));
	DOCTEST_CHECK(en::Math::Equals(en::Math::Sin(15.0f), 0.2588f));
	DOCTEST_CHECK(en::Math::Equals(en::Math::Sin(22.5f), 0.3826f));
	DOCTEST_CHECK(en::Math::Equals(en::Math::Sin(30.0f), 0.5f));
	DOCTEST_CHECK(en::Math::Equals(en::Math::Sin(45.0f), 0.7071f));
	DOCTEST_CHECK(en::Math::Equals(en::Math::Sin(60.0f), 0.8660f));
	DOCTEST_CHECK(en::Math::Equals(en::Math::Sin(67.5f), 0.9238f));
	DOCTEST_CHECK(en::Math::Equals(en::Math::Sin(75.0f), 0.9659f));
	DOCTEST_CHECK(en::Math::Equals(en::Math::Sin(90.0f), 1.0f));
	DOCTEST_CHECK(en::Math::Equals(en::Math::Sin(120.0f), 0.8660f));
	DOCTEST_CHECK(en::Math::Equals(en::Math::Sin(135.0f), 0.7071f));
	DOCTEST_CHECK(en::Math::Equals(en::Math::Sin(150.0f), 0.5f));
	DOCTEST_CHECK(en::Math::Equals(en::Math::Sin(180.0f), 0.0f));
	DOCTEST_CHECK(en::Math::Equals(en::Math::Sin(-0.0f), -0.0f));
	DOCTEST_CHECK(en::Math::Equals(en::Math::Sin(-15.0f), -0.2588f));
	DOCTEST_CHECK(en::Math::Equals(en::Math::Sin(-22.5f), -0.3826f));
	DOCTEST_CHECK(en::Math::Equals(en::Math::Sin(-30.0f), -0.5f));
	DOCTEST_CHECK(en::Math::Equals(en::Math::Sin(-45.0f), -0.7071f));
	DOCTEST_CHECK(en::Math::Equals(en::Math::Sin(-60.0f), -0.8660f));
	DOCTEST_CHECK(en::Math::Equals(en::Math::Sin(-67.5f), -0.9238f));
	DOCTEST_CHECK(en::Math::Equals(en::Math::Sin(-75.0f), -0.9659f));
	DOCTEST_CHECK(en::Math::Equals(en::Math::Sin(-90.0f), -1.0f));
	DOCTEST_CHECK(en::Math::Equals(en::Math::Sin(-120.0f), -0.8660f));
	DOCTEST_CHECK(en::Math::Equals(en::Math::Sin(-135.0f), -0.7071f));
	DOCTEST_CHECK(en::Math::Equals(en::Math::Sin(-150.0f), -0.5f));
	DOCTEST_CHECK(en::Math::Equals(en::Math::Sin(-180.0f), -0.0f));

	DOCTEST_CHECK(en::Math::Equals(en::Math::Tan(0.0f), 0.0f));
	DOCTEST_CHECK(en::Math::Equals(en::Math::Tan(15.0f), 0.2679f));
	DOCTEST_CHECK(en::Math::Equals(en::Math::Tan(22.5f), 0.4142f));
	DOCTEST_CHECK(en::Math::Equals(en::Math::Tan(30.0f), 0.5773f));
	DOCTEST_CHECK(en::Math::Equals(en::Math::Tan(45.0f), 1.0f));
	DOCTEST_CHECK(en::Math::Equals(en::Math::Tan(60.0f), 1.7320f));
	DOCTEST_CHECK(en::Math::Equals(en::Math::Tan(67.5f), 2.4142f));
	DOCTEST_CHECK(en::Math::Equals(en::Math::Tan(75.0f), 3.7320f));
	//DOCTEST_CHECK(en::Math::Equals(en::Math::Tan(90.0f), inf));
	DOCTEST_CHECK(en::Math::Equals(en::Math::Tan(120.0f), -1.7320f));
	DOCTEST_CHECK(en::Math::Equals(en::Math::Tan(135.0f), -1.0f));
	DOCTEST_CHECK(en::Math::Equals(en::Math::Tan(150.0f), -0.5773f));
	DOCTEST_CHECK(en::Math::Equals(en::Math::Tan(180.0f), 0.0f));
	DOCTEST_CHECK(en::Math::Equals(en::Math::Tan(-0.0f), 0.0f));
	DOCTEST_CHECK(en::Math::Equals(en::Math::Tan(-15.0f), -0.2679f));
	DOCTEST_CHECK(en::Math::Equals(en::Math::Tan(-22.5f), -0.4142f));
	DOCTEST_CHECK(en::Math::Equals(en::Math::Tan(-30.0f), -0.5773f));
	DOCTEST_CHECK(en::Math::Equals(en::Math::Tan(-45.0f), -1.0f));
	DOCTEST_CHECK(en::Math::Equals(en::Math::Tan(-60.0f), -1.7320f));
	DOCTEST_CHECK(en::Math::Equals(en::Math::Tan(-67.5f), -2.4142f));
	DOCTEST_CHECK(en::Math::Equals(en::Math::Tan(-75.0f), -3.7320f));
	//DOCTEST_CHECK(en::Math::Equals(en::Math::Tan(-90.0f), inf));
	DOCTEST_CHECK(en::Math::Equals(en::Math::Tan(-120.0f), 1.7320f));
	DOCTEST_CHECK(en::Math::Equals(en::Math::Tan(-135.0f), 1.0f));
	DOCTEST_CHECK(en::Math::Equals(en::Math::Tan(-150.0f), 0.5773f));
	DOCTEST_CHECK(en::Math::Equals(en::Math::Tan(-180.0f), 0.0f));

	DOCTEST_CHECK(en::Math::Equals(en::Math::Sqrt(1.0f), 1.0f));
	DOCTEST_CHECK(en::Math::Equals(en::Math::Sqrt(4.0f), 2.0f));
	DOCTEST_CHECK(en::Math::Equals(en::Math::Sqrt(9.0f), 3.0f));
	DOCTEST_CHECK(en::Math::Equals(en::Math::Sqrt(16.0f), 4.0f));
	DOCTEST_CHECK(en::Math::Equals(en::Math::Sqrt(25.0f), 5.0f));
	DOCTEST_CHECK(en::Math::Equals(en::Math::Sqrt(36.0f), 6.0f));
	DOCTEST_CHECK(en::Math::Equals(en::Math::Sqrt(49.0f), 7.0f));
	DOCTEST_CHECK(en::Math::Equals(en::Math::Sqrt(64.0f), 8.0f));
	DOCTEST_CHECK(en::Math::Equals(en::Math::Sqrt(81.0f), 9.0f));
	DOCTEST_CHECK(en::Math::Equals(en::Math::Sqrt(100.0f), 10.0f));

	DOCTEST_CHECK(en::Math::Equals(en::Math::InvSqrt(1.0f), 1.0f));
	DOCTEST_CHECK(en::Math::Equals(en::Math::InvSqrt(4.0f), 1.0f / 2.0f));
	DOCTEST_CHECK(en::Math::Equals(en::Math::InvSqrt(9.0f), 1.0f / 3.0f));
	DOCTEST_CHECK(en::Math::Equals(en::Math::InvSqrt(16.0f), 1.0f / 4.0f));
	DOCTEST_CHECK(en::Math::Equals(en::Math::InvSqrt(25.0f), 1.0f / 5.0f));
	DOCTEST_CHECK(en::Math::Equals(en::Math::InvSqrt(36.0f), 1.0f / 6.0f));
	DOCTEST_CHECK(en::Math::Equals(en::Math::InvSqrt(49.0f), 1.0f / 7.0f));
	DOCTEST_CHECK(en::Math::Equals(en::Math::InvSqrt(64.0f), 1.0f / 8.0f));
	DOCTEST_CHECK(en::Math::Equals(en::Math::InvSqrt(81.0f), 1.0f / 9.0f));
	DOCTEST_CHECK(en::Math::Equals(en::Math::InvSqrt(100.0f), 1.0f / 10.0f));

	// TODO : Improve tests
}