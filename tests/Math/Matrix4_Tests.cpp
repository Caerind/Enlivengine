#include <Enlivengine/Math/Matrix4.hpp>

#include <doctest/doctest.h>

DOCTEST_TEST_CASE("Matrix4")
{
	// Inverse
	en::Matrix4f invTest(2, 0, -1, 0, 0, 1, 0, 0, 1, 0, 1, 0, 0, 0, 0, 1);
	en::Matrix4f invResult(1 / 3.f, 0, 1 / 3.f, 0, 0, 1, 0, 0, -1 / 3.f, 0, 2 / 3.f, 0, 0, 0, 0, 1);
	DOCTEST_CHECK(invTest.inversed() == invResult);
	DOCTEST_CHECK(invTest.inverse() == invResult);
	DOCTEST_CHECK(invResult.inversed().inversed() == invResult);

	// Transpose
	en::Matrix4f transposeTest(0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15);
	en::Matrix4f transposeResult(0, 4, 8, 12, 1, 5, 9, 13, 2, 6, 10, 14, 3, 7, 11, 15);
	DOCTEST_CHECK(transposeTest.transposed() == transposeResult);
	DOCTEST_CHECK(transposeTest.transpose() == transposeResult);
	DOCTEST_CHECK(transposeResult.transposed().transposed() == transposeResult);

	// TODO : Improve tests
}