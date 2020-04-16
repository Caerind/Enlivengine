#include <Enlivengine/Math/Matrix3.hpp>

#include <doctest/doctest.h>

DOCTEST_TEST_CASE("Matrix3")
{
	// Inverse
	en::Matrix3f invTest(2, 0, -1, 0, 1, 0, 1, 0, 1);
	en::Matrix3f invResult(1 / 3.f, 0, 1 / 3.f, 0, 1, 0, -1 / 3.f, 0, 2 / 3.f);
	DOCTEST_CHECK(invTest.inversed() == invResult);
	DOCTEST_CHECK(invTest.inverse() == invResult);
	DOCTEST_CHECK(invResult.inversed().inversed() == invResult);

	// Transpose
	en::Matrix3f transposeTest(0, 1, 2, 3, 4, 5, 6, 7, 8);
	en::Matrix3f transposeResult(0, 3, 6, 1, 4, 7, 2, 5, 8);
	DOCTEST_CHECK(transposeTest.transposed() == transposeResult);
	DOCTEST_CHECK(transposeTest.transpose() == transposeResult);
	DOCTEST_CHECK(transposeResult.transposed().transposed() == transposeResult);

	// TODO : Improve tests
}