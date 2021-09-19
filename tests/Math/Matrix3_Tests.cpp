#include <Enlivengine/Math/Matrix3.hpp>

#include <doctest/doctest.h>

DOCTEST_TEST_CASE("Matrix3")
{
	const en::Matrix3f layout(0, 1, 2, 3, 4, 5, 6, 7, 8);
	en::Matrix3f tmp;

	// Identity and equality check
	DOCTEST_CHECK(en::Matrix3f::Identity() == en::Matrix3f::Identity());
	DOCTEST_CHECK(layout != en::Matrix3f::Identity());
	DOCTEST_CHECK(en::Matrix3f(1, 0, 0, 0, 1, 0, 0, 0, 1) == en::Matrix3f::Identity());
	DOCTEST_CHECK(en::Matrix3f::Identity().IsIdentity());
	DOCTEST_CHECK(!layout.IsIdentity());

	// Ctors
	DOCTEST_CHECK(en::Matrix3f() == en::Matrix3f::Identity());
	DOCTEST_CHECK(en::Matrix3f(en::Matrix3f::Identity()) == en::Matrix3f::Identity());
	DOCTEST_CHECK(en::Matrix3f(layout) == layout);

	// Layout access
	DOCTEST_CHECK(en::Matrix3f::Elements == 9);
	DOCTEST_CHECK(en::Matrix3f::Rows == 3);
	DOCTEST_CHECK(en::Matrix3f::Columns == 3);
	DOCTEST_CHECK(layout.GetColumn(0) == en::Vector3f(0, 1, 2));
	DOCTEST_CHECK(layout.GetColumn(1) == en::Vector3f(3, 4, 5));
	DOCTEST_CHECK(layout.GetColumn(2) == en::Vector3f(6, 7, 8));
	DOCTEST_CHECK(layout.GetRow(0) == en::Vector3f(0, 3, 6));
	DOCTEST_CHECK(layout.GetRow(1) == en::Vector3f(1, 4, 7));
	DOCTEST_CHECK(layout.GetRow(2) == en::Vector3f(2, 5, 8));

	// GetData
	const en::Matrix3f dataA = layout;
	const en::Matrix3f dataB = en::Matrix3f::Identity() * 3.0f;
	DOCTEST_CHECK(dataA.GetData() != dataB.GetData());
	const en::F32 dataC[9] = { 0.0f, 1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f, 7.0f, 8.0f };
	for (int i = 0; i < 9; ++i)
	{
		DOCTEST_CHECK(dataA.GetData()[i] == dataC[i]);
	}

	// Operators 
	DOCTEST_CHECK(layout == +layout);
	DOCTEST_CHECK(en::Matrix3f::Identity() == +en::Matrix3f::Identity());
	DOCTEST_CHECK(layout == -(-layout));
	DOCTEST_CHECK(en::Matrix3f::Identity() == -(-en::Matrix3f::Identity()));
	DOCTEST_CHECK(en::Matrix3f::Identity() + layout == layout + en::Matrix3f::Identity());
	DOCTEST_CHECK(en::Matrix3f::Identity() - layout != layout - en::Matrix3f::Identity());
	DOCTEST_CHECK(en::Matrix3f::Identity() + -layout == en::Matrix3f::Identity() - layout);
	DOCTEST_CHECK(-en::Matrix3f::Identity() + layout == layout - en::Matrix3f::Identity());
	DOCTEST_CHECK(en::Matrix3f::Identity() + layout == en::Matrix3f(1, 1, 2, 3, 5, 5, 6, 7, 9));
	DOCTEST_CHECK(en::Matrix3f::Identity() - layout == en::Matrix3f(1, -1, -2, -3, -3, -5, -6, -7, -7));
	DOCTEST_CHECK(layout - en::Matrix3f::Identity() == en::Matrix3f(-1, 1, 2, 3, 3, 5, 6, 7, 7));
	DOCTEST_CHECK(en::Matrix3f::Identity() * layout == layout);
	DOCTEST_CHECK(layout * en::Matrix3f::Identity() == layout);
	const en::Matrix3f other(8, 7, 6, 5, 4, 3, 2, 1, 0);
	DOCTEST_CHECK(other * layout != layout * other);
	DOCTEST_CHECK(layout * other == en::Matrix3f(57, 78, 99, 30, 42, 54, 3, 6, 9));
	DOCTEST_CHECK(other * layout == en::Matrix3f(9, 6, 3, 54, 42, 30, 99, 78, 57));
	DOCTEST_CHECK(en::Matrix3f(10, 20, 10, 4, 5, 6, 2, 3, 5) * en::Matrix3f(3, 2, 4, 3, 3, 9, 4, 4, 2) == en::Matrix3f(46, 82, 62, 60, 102, 93, 60, 106, 74));
	en::Matrix3f l1 = layout; l1 += en::Matrix3f::Identity();
	en::Matrix3f l2 = layout; l2 -= en::Matrix3f::Identity();
	en::Matrix3f l3 = layout; l3 *= en::Matrix3f::Identity();
	en::Matrix3f l4 = layout; l4 *= other;
	en::Matrix3f l5 = other; l5 *= layout;
	en::Matrix3f l6 = layout; l6 *= 2.0f;
	en::Matrix3f l7 = layout; l7 /= 2.0f;
	DOCTEST_CHECK(l1 == layout + en::Matrix3f::Identity());
	DOCTEST_CHECK(l2 == layout - en::Matrix3f::Identity());
	DOCTEST_CHECK(l3 == layout * en::Matrix3f::Identity());
	DOCTEST_CHECK(l4 == layout * other);
	DOCTEST_CHECK(l5 == other * layout);
	DOCTEST_CHECK(layout * 2.0f == en::Matrix3f(0, 2, 4, 6, 8, 10, 12, 14, 16));
	DOCTEST_CHECK(layout / 2.0f == en::Matrix3f(0, 1 / 2.0f, 2 / 2.0f, 3 / 2.0f, 4 / 2.0f, 5 / 2.0f, 6 / 2.0f, 7 / 2.0f, 8 / 2.0f));
	DOCTEST_CHECK(l6 == en::Matrix3f(0, 2, 4, 6, 8, 10, 12, 14, 16));
	DOCTEST_CHECK(l7 == en::Matrix3f(0, 1 / 2.0f, 2 / 2.0f, 3 / 2.0f, 4 / 2.0f, 5 / 2.0f, 6 / 2.0f, 7 / 2.0f, 8 / 2.0f));

	// Orthogonal
	DOCTEST_CHECK(!layout.IsOrthogonal());
	//DOCTEST_CHECK(en::Matrix3f::RotationX(26.0f).IsOrthogonal());
	//DOCTEST_CHECK(en::Matrix3f::RotationY(26.0f).IsOrthogonal());
	//DOCTEST_CHECK(en::Matrix3f::RotationZ(26.0f).IsOrthogonal());
	//DOCTEST_CHECK((en::Matrix3f::RotationX(26.0f) * en::Matrix3f::Identity()).IsOrthonormal());
	//DOCTEST_CHECK((en::Matrix3f::RotationX(26.0f) * en::Matrix3f::RotationY(26.0f) * en::Matrix3f::RotationZ(26.0f)).IsOrthonormal());
	//DOCTEST_CHECK(!(en::Matrix3f::RotationX(26.0f) + en::Matrix3f::Identity()).IsOrthonormal());

	// Trace
	DOCTEST_CHECK(en::Matrix3f::Identity().GetTrace() == 3);
	DOCTEST_CHECK(layout.GetTrace() == 12);

	// Determinant
	DOCTEST_CHECK(en::Matrix3f::Identity().GetDeterminant() == 1);
	DOCTEST_CHECK(layout.GetDeterminant() == 0);

	// Inversion
	en::Matrix3f invTest(2, 0, -1, 0, 1, 0, 1, 0, 1);
	en::Matrix3f invResult(1 / 3.0f, 0, 1 / 3.0f, 0, 1, 0, -1 / 3.0f, 0, 2 / 3.0f);
	DOCTEST_CHECK(invTest.Inversed() == invResult);
	DOCTEST_CHECK(invTest.Inverse() == invResult);
	DOCTEST_CHECK(en::Matrix3f::Equals(invResult.Inversed().Inversed(), invResult));
	//DOCTEST_CHECK(layout.Inversed().IsIdentity()); // Layout is not inversible

	// Transposition
	en::Matrix3f transposeTest(0, 1, 2, 3, 4, 5, 6, 7, 8);
	en::Matrix3f transposeResult(0, 3, 6, 1, 4, 7, 2, 5, 8);
	DOCTEST_CHECK(transposeTest.Transposed() == transposeResult);
	DOCTEST_CHECK(transposeTest.Transpose() == transposeResult);
	DOCTEST_CHECK(transposeResult.Transposed().Transposed() == transposeResult);

	// TransformDirection
	en::Vector3f vTmp;
	vTmp = en::Matrix3f::RotationY(90.0f) * en::Vector3f::UnitX();
	DOCTEST_CHECK(en::Vector3f::Equals(vTmp, -en::Vector3f::UnitZ()));
	vTmp = en::Matrix3f::RotationZ(90.0f) * en::Vector3f::UnitY();
	DOCTEST_CHECK(en::Vector3f::Equals(vTmp, -en::Vector3f::UnitX()));
	vTmp = en::Matrix3f::RotationX(90.0f) * en::Vector3f::UnitZ();
	DOCTEST_CHECK(en::Vector3f::Equals(vTmp, -en::Vector3f::UnitY()));

	// Rotations
	en::F32 c = en::Math::Cos(26.0f);
	en::F32 s = en::Math::Sin(26.0f);
	DOCTEST_CHECK(en::Matrix3f::RotationX(26.0f) == en::Matrix3f(1, 0, 0, 0, c, s, 0, -s, c));
	DOCTEST_CHECK(en::Matrix3f::RotationY(26.0f) == en::Matrix3f(c, 0, -s, 0, 1, 0, s, 0, c));
	DOCTEST_CHECK(en::Matrix3f::RotationZ(26.0f) == en::Matrix3f(c, s, 0, -s, c, 0, 0, 0, 1));
}