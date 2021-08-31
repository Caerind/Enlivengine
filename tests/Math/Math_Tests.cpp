#include <Enlivengine/Math/Math.hpp>

#include <doctest/doctest.h>

DOCTEST_TEST_CASE("Math")
{
	DOCTEST_CHECK_EQ(en::Math::Floor(2.0f), 2);
	DOCTEST_CHECK_EQ(en::Math::Ceil(2.0f), 2);
	DOCTEST_CHECK_EQ(en::Math::Floor(2.4f), 2);
	DOCTEST_CHECK_EQ(en::Math::Ceil(2.4f), 3);
	DOCTEST_CHECK_EQ(en::Math::Floor(-2.7f), -3);
	DOCTEST_CHECK_EQ(en::Math::Ceil(-2.7f), -2);
	DOCTEST_CHECK_EQ(en::Math::Floor(-2.0f), -2);
	DOCTEST_CHECK_EQ(en::Math::Ceil(-2.0f), -2);

	DOCTEST_CHECK_EQ(en::Math::Clamp(50, 0, 10), 10);
	DOCTEST_CHECK_EQ(en::Math::Clamp(50, 0, 100), 50);
	DOCTEST_CHECK_EQ(en::Math::Clamp(50, 100, 1000), 100);
	DOCTEST_CHECK(en::Math::InRange(50, 0, 100));
	DOCTEST_CHECK(en::Math::Equals(50.0000f, 50.0001f, 0.001f));

	DOCTEST_CHECK_EQ(en::Math::Sqrt(1.0f), 1.0f);
	DOCTEST_CHECK_EQ(en::Math::Sqrt(4.0f), 2.0f);
	DOCTEST_CHECK_EQ(en::Math::Sqrt(9.0f), 3.0f);
	DOCTEST_CHECK_EQ(en::Math::Sqrt(16.0f), 4.0f);
	DOCTEST_CHECK_EQ(en::Math::Sqrt(25.0f), 5.0f);
	DOCTEST_CHECK_EQ(en::Math::Sqrt(36.0f), 6.0f);
	DOCTEST_CHECK_EQ(en::Math::Sqrt(49.0f), 7.0f);
	DOCTEST_CHECK_EQ(en::Math::Sqrt(64.0f), 8.0f);
	DOCTEST_CHECK_EQ(en::Math::Sqrt(81.0f), 9.0f);
	DOCTEST_CHECK_EQ(en::Math::Sqrt(100.0f), 10.0f);

	DOCTEST_CHECK_EQ(en::Math::InvSqrt(1.0f), 1.0f);
	DOCTEST_CHECK_EQ(en::Math::InvSqrt(4.0f), 1.0f / 2.0f);
	DOCTEST_CHECK_EQ(en::Math::InvSqrt(9.0f), 1.0f / 3.0f);
	DOCTEST_CHECK_EQ(en::Math::InvSqrt(16.0f), 1.0f / 4.0f);
	DOCTEST_CHECK_EQ(en::Math::InvSqrt(25.0f), 1.0f / 5.0f);
	DOCTEST_CHECK_EQ(en::Math::InvSqrt(36.0f), 1.0f / 6.0f);
	DOCTEST_CHECK_EQ(en::Math::InvSqrt(49.0f), 1.0f / 7.0f);
	DOCTEST_CHECK_EQ(en::Math::InvSqrt(64.0f), 1.0f / 8.0f);
	DOCTEST_CHECK_EQ(en::Math::InvSqrt(81.0f), 1.0f / 9.0f);
	DOCTEST_CHECK_EQ(en::Math::InvSqrt(100.0f), 1.0f / 10.0f);

	/*
	DOCTEST_CHECK_EQ(en::Math::Cos(0.0f), 1.0f);
	DOCTEST_CHECK_EQ(en::Math::Cos(15.0f), 0.96593f);
	DOCTEST_CHECK_EQ(en::Math::Cos(22.5f), 0.9238f);
	DOCTEST_CHECK_EQ(en::Math::Cos(30.0f), 0.8660f);
	DOCTEST_CHECK_EQ(en::Math::Cos(45.0f), 0.7071f);
	DOCTEST_CHECK_EQ(en::Math::Cos(60.0f), 0.5f);
	DOCTEST_CHECK_EQ(en::Math::Cos(67.5f), 0.3826f);
	DOCTEST_CHECK_EQ(en::Math::Cos(75.0f), 0.2588f);
	DOCTEST_CHECK_EQ(en::Math::Cos(90.0f), 0.0f);
	DOCTEST_CHECK_EQ(en::Math::Cos(120.0f), -0.5f);
	DOCTEST_CHECK_EQ(en::Math::Cos(135.0f), -0.7071f);
	DOCTEST_CHECK_EQ(en::Math::Cos(150.0f), -0.8660f);
	DOCTEST_CHECK_EQ(en::Math::Cos(180.0f), -1.0f);
	DOCTEST_CHECK_EQ(en::Math::Cos(-0.0f), 1.0f);
	DOCTEST_CHECK_EQ(en::Math::Cos(-15.0f), 0.9659f);
	DOCTEST_CHECK_EQ(en::Math::Cos(-22.5f), 0.9238f);
	DOCTEST_CHECK_EQ(en::Math::Cos(-30.0f), 0.8660f);
	DOCTEST_CHECK_EQ(en::Math::Cos(-45.0f), 0.7071f);
	DOCTEST_CHECK_EQ(en::Math::Cos(-60.0f), 0.5f);
	DOCTEST_CHECK_EQ(en::Math::Cos(-67.5f), 0.3826f);
	DOCTEST_CHECK_EQ(en::Math::Cos(-75.0f), 0.2588f);
	DOCTEST_CHECK_EQ(en::Math::Cos(-90.0f), 0.0f);
	DOCTEST_CHECK_EQ(en::Math::Cos(-120.0f), -0.5f);
	DOCTEST_CHECK_EQ(en::Math::Cos(-135.0f), -0.7071f);
	DOCTEST_CHECK_EQ(en::Math::Cos(-150.0f), -0.8660f);
	DOCTEST_CHECK_EQ(en::Math::Cos(-180.0f), -1.0f);

	DOCTEST_CHECK_EQ(en::Math::Sin(0.0f), 0.0f);
	DOCTEST_CHECK_EQ(en::Math::Sin(15.0f), 0.2588f);
	DOCTEST_CHECK_EQ(en::Math::Sin(22.5f), 0.3826f);
	DOCTEST_CHECK_EQ(en::Math::Sin(30.0f), 0.5f);
	DOCTEST_CHECK_EQ(en::Math::Sin(45.0f), 0.7071f);
	DOCTEST_CHECK_EQ(en::Math::Sin(60.0f), 0.8660f);
	DOCTEST_CHECK_EQ(en::Math::Sin(67.5f), 0.9238f);
	DOCTEST_CHECK_EQ(en::Math::Sin(75.0f), 0.9659f);
	DOCTEST_CHECK_EQ(en::Math::Sin(90.0f), 1.0f);
	DOCTEST_CHECK_EQ(en::Math::Sin(120.0f), 0.8660f);
	DOCTEST_CHECK_EQ(en::Math::Sin(135.0f), 0.7071f);
	DOCTEST_CHECK_EQ(en::Math::Sin(150.0f), 0.5f);
	DOCTEST_CHECK_EQ(en::Math::Sin(180.0f), 0.0f);
	DOCTEST_CHECK_EQ(en::Math::Sin(-0.0f), -0.0f);
	DOCTEST_CHECK_EQ(en::Math::Sin(-15.0f), -0.2588f);
	DOCTEST_CHECK_EQ(en::Math::Sin(-22.5f), -0.3826f);
	DOCTEST_CHECK_EQ(en::Math::Sin(-30.0f), -0.5f);
	DOCTEST_CHECK_EQ(en::Math::Sin(-45.0f), -0.7071f);
	DOCTEST_CHECK_EQ(en::Math::Sin(-60.0f), -0.8660f);
	DOCTEST_CHECK_EQ(en::Math::Sin(-67.5f), -0.9238f);
	DOCTEST_CHECK_EQ(en::Math::Sin(-75.0f), -0.9659f);
	DOCTEST_CHECK_EQ(en::Math::Sin(-90.0f), -1.0f);
	DOCTEST_CHECK_EQ(en::Math::Sin(-120.0f), -0.8660f);
	DOCTEST_CHECK_EQ(en::Math::Sin(-135.0f), -0.7071f);
	DOCTEST_CHECK_EQ(en::Math::Sin(-150.0f), -0.5f);
	DOCTEST_CHECK_EQ(en::Math::Sin(-180.0f), -0.0f);

	DOCTEST_CHECK_EQ(en::Math::Tan(0.0f), 0.0f);
	DOCTEST_CHECK_EQ(en::Math::Tan(15.0f), 0.2679f);
	DOCTEST_CHECK_EQ(en::Math::Tan(22.5f), 0.4142f);
	DOCTEST_CHECK_EQ(en::Math::Tan(30.0f), 0.5773f);
	DOCTEST_CHECK_EQ(en::Math::Tan(45.0f), 1.0f);
	DOCTEST_CHECK_EQ(en::Math::Tan(60.0f), 1.7320f);
	DOCTEST_CHECK_EQ(en::Math::Tan(67.5f), 2.4142f);
	DOCTEST_CHECK_EQ(en::Math::Tan(75.0f), 3.7320f);
	//DOCTEST_CHECK_EQ(en::Math::Tan(90.0f), inf);
	DOCTEST_CHECK_EQ(en::Math::Tan(120.0f), -1.7320f);
	DOCTEST_CHECK_EQ(en::Math::Tan(135.0f), -1.0f);
	DOCTEST_CHECK_EQ(en::Math::Tan(150.0f), -0.5773f);
	DOCTEST_CHECK_EQ(en::Math::Tan(180.0f), 0.0f);
	DOCTEST_CHECK_EQ(en::Math::Tan(-0.0f), 0.0f);
	DOCTEST_CHECK_EQ(en::Math::Tan(-15.0f), -0.2679f);
	DOCTEST_CHECK_EQ(en::Math::Tan(-22.5f), -0.4142f);
	DOCTEST_CHECK_EQ(en::Math::Tan(-30.0f), -0.5773f);
	DOCTEST_CHECK_EQ(en::Math::Tan(-45.0f), -1.0f);
	DOCTEST_CHECK_EQ(en::Math::Tan(-60.0f), -1.7320f);
	DOCTEST_CHECK_EQ(en::Math::Tan(-67.5f), -2.4142f);
	DOCTEST_CHECK_EQ(en::Math::Tan(-75.0f), -3.7320f);
	//DOCTEST_CHECK_EQ(en::Math::Tan(-90.0f), inf);
	DOCTEST_CHECK_EQ(en::Math::Tan(-120.0f), 1.7320f);
	DOCTEST_CHECK_EQ(en::Math::Tan(-135.0f), 1.0f);
	DOCTEST_CHECK_EQ(en::Math::Tan(-150.0f), 0.5773f);
	DOCTEST_CHECK_EQ(en::Math::Tan(-180.0f), 0.0f);

	DOCTEST_CHECK_EQ(en::Math::Asin(-1.0f), -90.0f);
	DOCTEST_CHECK_EQ(en::Math::Asin(-0.9f), -64.1580f);
	DOCTEST_CHECK_EQ(en::Math::Asin(-0.8f), -53.1301f);
	DOCTEST_CHECK_EQ(en::Math::Asin(-0.7f), -44.4270f);
	DOCTEST_CHECK_EQ(en::Math::Asin(-0.6f), -36.8698f);
	DOCTEST_CHECK_EQ(en::Math::Asin(-0.5f), -30.0f);
	DOCTEST_CHECK_EQ(en::Math::Asin(-0.4f), -23.5781f);
	DOCTEST_CHECK_EQ(en::Math::Asin(-0.3f), -17.4576f);
	DOCTEST_CHECK_EQ(en::Math::Asin(-0.2f), -11.5369f);
	DOCTEST_CHECK_EQ(en::Math::Asin(-0.1f), -5.7391f);
	DOCTEST_CHECK_EQ(en::Math::Asin(-0.0f), 0.0f);
	DOCTEST_CHECK_EQ(en::Math::Asin(0.0f), 0.0f);
	DOCTEST_CHECK_EQ(en::Math::Asin(0.1f), 5.7391f);
	DOCTEST_CHECK_EQ(en::Math::Asin(0.2f), 11.5369f);
	DOCTEST_CHECK_EQ(en::Math::Asin(0.3f), 17.4576f);
	DOCTEST_CHECK_EQ(en::Math::Asin(0.4f), 23.5781f);
	DOCTEST_CHECK_EQ(en::Math::Asin(0.5f), 30.0f);
	DOCTEST_CHECK_EQ(en::Math::Asin(0.6f), 36.8698f);
	DOCTEST_CHECK_EQ(en::Math::Asin(0.7f), 44.4270f);
	DOCTEST_CHECK_EQ(en::Math::Asin(0.8f), 53.1301f);
	DOCTEST_CHECK_EQ(en::Math::Asin(0.9f), 64.1580f);
	DOCTEST_CHECK_EQ(en::Math::Asin(1.0f), 90.0f);

	DOCTEST_CHECK_EQ(en::Math::Acos(-1.0f), 180.0f);
	DOCTEST_CHECK_EQ(en::Math::Acos(-0.9f), 154.1580f);
	DOCTEST_CHECK_EQ(en::Math::Acos(-0.8f), 143.1301f);
	DOCTEST_CHECK_EQ(en::Math::Acos(-0.7f), 134.4270f);
	DOCTEST_CHECK_EQ(en::Math::Acos(-0.6f), 126.8698f);
	DOCTEST_CHECK_EQ(en::Math::Acos(-0.5f), 120.0f);
	DOCTEST_CHECK_EQ(en::Math::Acos(-0.4f), 113.5781f);
	DOCTEST_CHECK_EQ(en::Math::Acos(-0.3f), 107.4576f);
	DOCTEST_CHECK_EQ(en::Math::Acos(-0.2f), 101.5369f);
	DOCTEST_CHECK_EQ(en::Math::Acos(-0.1f), 95.7391f);
	DOCTEST_CHECK_EQ(en::Math::Acos(-0.0f), 90.0f);
	DOCTEST_CHECK_EQ(en::Math::Acos(0.0f), 90.0f);
	DOCTEST_CHECK_EQ(en::Math::Acos(0.1f), 84.2608f);
	DOCTEST_CHECK_EQ(en::Math::Acos(0.2f), 78.4630f);
	DOCTEST_CHECK_EQ(en::Math::Acos(0.3f), 72.54239f);
	DOCTEST_CHECK_EQ(en::Math::Acos(0.4f), 66.4218f);
	DOCTEST_CHECK_EQ(en::Math::Acos(0.5f), 60.0f);
	DOCTEST_CHECK_EQ(en::Math::Acos(0.6f), 53.1301f);
	DOCTEST_CHECK_EQ(en::Math::Acos(0.7f), 45.5729f);
	DOCTEST_CHECK_EQ(en::Math::Acos(0.8f), 36.8698f);
	DOCTEST_CHECK_EQ(en::Math::Acos(0.9f), 25.8419f);
	DOCTEST_CHECK_EQ(en::Math::Acos(1.0f), 0.0f);

	DOCTEST_CHECK_EQ(en::Math::Atan(-1000.0f), -89.9427f);
	DOCTEST_CHECK_EQ(en::Math::Atan(-100.0f), -89.4270f);
	DOCTEST_CHECK_EQ(en::Math::Atan(-10.0f), -84.2894f);
	DOCTEST_CHECK_EQ(en::Math::Atan(-3.0f), -71.5650f);
	DOCTEST_CHECK_EQ(en::Math::Atan(-2.0f), -63.4349f);
	DOCTEST_CHECK_EQ(en::Math::Atan(-1.6f), -57.9946f);
	DOCTEST_CHECK_EQ(en::Math::Atan(-1.3f), -52.4314f);
	DOCTEST_CHECK_EQ(en::Math::Atan(-1.0f), -45.0f);
	DOCTEST_CHECK_EQ(en::Math::Atan(-0.9f), -41.9872f);
	DOCTEST_CHECK_EQ(en::Math::Atan(-0.8f), -38.6598f);
	DOCTEST_CHECK_EQ(en::Math::Atan(-0.7f), -34.9920f);
	DOCTEST_CHECK_EQ(en::Math::Atan(-0.6f), -30.9637f);
	DOCTEST_CHECK_EQ(en::Math::Atan(-0.5f), -26.5650f);
	DOCTEST_CHECK_EQ(en::Math::Atan(-0.4f), -21.8014f);
	DOCTEST_CHECK_EQ(en::Math::Atan(-0.3f), -16.6992f);
	DOCTEST_CHECK_EQ(en::Math::Atan(-0.2f), -11.3099f);
	DOCTEST_CHECK_EQ(en::Math::Atan(-0.1f), -5.7105f);
	DOCTEST_CHECK_EQ(en::Math::Atan(-0.0f), 0.0f);
	DOCTEST_CHECK_EQ(en::Math::Atan(0.0f), 0.0f);
	DOCTEST_CHECK_EQ(en::Math::Atan(0.1f), 5.7105f);
	DOCTEST_CHECK_EQ(en::Math::Atan(0.2f), 11.3099f);
	DOCTEST_CHECK_EQ(en::Math::Atan(0.3f), 16.6992f);
	DOCTEST_CHECK_EQ(en::Math::Atan(0.4f), 21.8014f);
	DOCTEST_CHECK_EQ(en::Math::Atan(0.5f), 26.5650f);
	DOCTEST_CHECK_EQ(en::Math::Atan(0.6f), 30.9637f);
	DOCTEST_CHECK_EQ(en::Math::Atan(0.7f), 34.9920f);
	DOCTEST_CHECK_EQ(en::Math::Atan(0.8f), 38.6598f);
	DOCTEST_CHECK_EQ(en::Math::Atan(0.9f), 41.9872f);
	DOCTEST_CHECK_EQ(en::Math::Atan(1.0f), 45.0f);
	DOCTEST_CHECK_EQ(en::Math::Atan(1.3f), 52.4314f);
	DOCTEST_CHECK_EQ(en::Math::Atan(1.6f), 57.9946f);
	DOCTEST_CHECK_EQ(en::Math::Atan(2.0f), 63.4349f);
	DOCTEST_CHECK_EQ(en::Math::Atan(3.0f), 71.5650f);
	DOCTEST_CHECK_EQ(en::Math::Atan(10.0f), 84.2894f);
	DOCTEST_CHECK_EQ(en::Math::Atan(100.0f), 89.4270f);
	DOCTEST_CHECK_EQ(en::Math::Atan(1000.0f), 89.9427f);

	DOCTEST_CHECK_EQ(en::Math::Atan2(0.0f, 1.0f), 90.0f);
	DOCTEST_CHECK_EQ(en::Math::Atan2(20.000000f, 10.000000f), 26.5650f);
	DOCTEST_CHECK_EQ(en::Math::Atan2(10.000000f, 20.000000f), 63.4349f);
	DOCTEST_CHECK_EQ(en::Math::Atan2(-10.000000f, 20.000000f), 116.5650f);
	DOCTEST_CHECK_EQ(en::Math::Atan2(-20.000000f, 10.000000f), 153.4349f);
	DOCTEST_CHECK_EQ(en::Math::Atan2(-20.000000f, -10.000000f), -153.4349f);
	DOCTEST_CHECK_EQ(en::Math::Atan2(-10.000000f, -20.000000f), -116.5650f);
	DOCTEST_CHECK_EQ(en::Math::Atan2(10.000000f, -20.000000f), -63.4349f);
	DOCTEST_CHECK_EQ(en::Math::Atan2(20.000000f, -10.000000f), -26.5650f);
	*/
}