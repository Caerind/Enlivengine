#include <Enlivengine/Math/Matrix3.hpp>

#include <doctest/doctest.h>

DOCTEST_TEST_CASE("Matrix3")
{
	// 0 1 2     a11 a12 a13
	// 3 4 5  =  a21 a22 a23
	// 6 7 8     a31 a32 a33
	const en::Matrix3f layout(0, 1, 2, 3, 4, 5, 6, 7, 8);
	DOCTEST_CHECK(layout.GetRow(0) == Vector3f(0, 1, 2));
	DOCTEST_CHECK(layout.GetRow(1) == Vector3f(3, 4, 5));
	DOCTEST_CHECK(layout.GetRow(2) == Vector3f(6, 7, 8));
	DOCTEST_CHECK(layout.GetColumn(0) == Vector3f(0, 3, 6));
	DOCTEST_CHECK(layout.GetColumn(1) == Vector3f(1, 4, 7));
	DOCTEST_CHECK(layout.GetColumn(2) == Vector3f(2, 5, 8));

	// Inverse
	en::Matrix3f invTest(2, 0, -1, 0, 1, 0, 1, 0, 1);
	en::Matrix3f invResult(1 / 3.f, 0, 1 / 3.f, 0, 1, 0, -1 / 3.f, 0, 2 / 3.f);
	DOCTEST_CHECK(invTest.Inversed() == invResult);
	DOCTEST_CHECK(invTest.Inverse() == invResult);
	DOCTEST_CHECK(invResult.Inversed().Inversed() == invResult);

	// Transpose
	en::Matrix3f transposeTest(0, 1, 2, 3, 4, 5, 6, 7, 8);
	en::Matrix3f transposeResult(0, 3, 6, 1, 4, 7, 2, 5, 8);
	DOCTEST_CHECK(transposeTest.Transposed() == transposeResult);
	DOCTEST_CHECK(transposeTest.Transpose() == transposeResult);
	DOCTEST_CHECK(transposeResult.Transposed().Transposed() == transposeResult);

	// TODO : Improve tests
}