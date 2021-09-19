#include <Enlivengine/Math/Matrix4.hpp>

#include <doctest/doctest.h>

DOCTEST_TEST_CASE("Matrix4")
{
	const en::Matrix4f layout(0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15);
	en::Matrix3f tmp;

	// Identity and equality check
	DOCTEST_CHECK(en::Matrix4f::Identity() == en::Matrix4f::Identity());
	DOCTEST_CHECK(layout != en::Matrix4f::Identity());
	DOCTEST_CHECK(en::Matrix4f(1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1) == en::Matrix4f::Identity());
	DOCTEST_CHECK(en::Matrix4f::Identity().IsIdentity());
	DOCTEST_CHECK(!layout.IsIdentity());

	// Ctors
	DOCTEST_CHECK(en::Matrix4f() == en::Matrix4f::Identity());
	DOCTEST_CHECK(en::Matrix4f(en::Matrix4f::Identity()) == en::Matrix4f::Identity());
	DOCTEST_CHECK(en::Matrix4f(layout) == layout);

	// Layout access
	DOCTEST_CHECK(en::Matrix4f::Elements == 16);
	DOCTEST_CHECK(en::Matrix4f::Rows == 4);
	DOCTEST_CHECK(en::Matrix4f::Columns == 4);
	DOCTEST_CHECK(layout.GetColumn(0) == en::Vector4f(0, 1, 2, 3));
	DOCTEST_CHECK(layout.GetColumn(1) == en::Vector4f(4, 5, 6, 7));
	DOCTEST_CHECK(layout.GetColumn(2) == en::Vector4f(8, 9, 10, 11));
	DOCTEST_CHECK(layout.GetColumn(3) == en::Vector4f(12, 13, 14, 15));
	DOCTEST_CHECK(layout.GetRow(0) == en::Vector4f(0, 4, 8, 12));
	DOCTEST_CHECK(layout.GetRow(1) == en::Vector4f(1, 5, 9, 13));
	DOCTEST_CHECK(layout.GetRow(2) == en::Vector4f(2, 6, 10, 14));
	DOCTEST_CHECK(layout.GetRow(3) == en::Vector4f(3, 7, 11, 15));

	// GetData
	const en::Matrix4f dataA = layout;
	const en::Matrix4f dataB = en::Matrix4f::Identity() * 3.0f;
	DOCTEST_CHECK(dataA.GetData() != dataB.GetData());
	const en::F32 dataC[16] = { 0.0f, 1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f, 7.0f, 8.0f, 9.0f, 10.0f, 11.0f, 12.0f, 13.0f, 14.0f, 15.0f };
	for (int i = 0; i < 16; ++i)
	{
		DOCTEST_CHECK(dataA.GetData()[i] == dataC[i]);
	}

	// Operators 
	DOCTEST_CHECK(layout == +layout);
	DOCTEST_CHECK(en::Matrix4f::Identity() == +en::Matrix4f::Identity());
	DOCTEST_CHECK(layout == -(-layout));
	DOCTEST_CHECK(en::Matrix4f::Identity() == -(-en::Matrix4f::Identity()));
	DOCTEST_CHECK(en::Matrix4f::Identity() + layout == layout + en::Matrix4f::Identity());
	DOCTEST_CHECK(en::Matrix4f::Identity() - layout != layout - en::Matrix4f::Identity());
	DOCTEST_CHECK(en::Matrix4f::Identity() + -layout == en::Matrix4f::Identity() - layout);
	DOCTEST_CHECK(-en::Matrix4f::Identity() + layout == layout - en::Matrix4f::Identity());
	DOCTEST_CHECK(en::Matrix4f::Identity() + layout == en::Matrix4f(1, 1, 2, 3, 4, 6, 6, 7, 8, 9, 11, 11, 12, 13, 14, 16));
	DOCTEST_CHECK(en::Matrix4f::Identity() - layout == en::Matrix4f(1, -1, -2, -3, -4, -4, -6, -7, -8, -9, -9, -11, -12, -13, -14, -14));
	DOCTEST_CHECK(layout - en::Matrix4f::Identity() == en::Matrix4f(-1, 1, 2, 3, 4, 4, 6, 7, 8, 9, 9, 11, 12, 13, 14, 14));
	DOCTEST_CHECK(en::Matrix4f::Identity() * layout == layout);
	DOCTEST_CHECK(layout * en::Matrix4f::Identity() == layout);
	const en::Matrix4f other(15, 14, 13, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1, 0);
	DOCTEST_CHECK(other * layout != layout * other);
	DOCTEST_CHECK(layout * other == en::Matrix4f(304, 358, 412, 466, 208, 246, 284, 322, 112, 134, 156, 178, 16, 22, 28, 34));
	DOCTEST_CHECK(other * layout == en::Matrix4f(34, 28, 22, 16, 178, 156, 134, 112, 322, 284, 246, 208, 466, 412, 358, 304));
	DOCTEST_CHECK(en::Matrix4f(5, 7, 9, 10, 2, 3, 3, 8, 8, 10, 2, 3, 3, 3, 4, 8) * en::Matrix4f(3, 10, 12, 18, 12, 1, 4, 9, 9, 10, 12, 2, 3, 12, 4, 10) == en::Matrix4f(185, 225, 153, 290, 121, 154, 155, 212, 167, 219, 143, 222, 101, 127, 111, 218));
	en::Matrix4f l1 = layout; l1 += en::Matrix4f::Identity();
	en::Matrix4f l2 = layout; l2 -= en::Matrix4f::Identity();
	en::Matrix4f l3 = layout; l3 *= en::Matrix4f::Identity();
	en::Matrix4f l4 = layout; l4 *= other;
	en::Matrix4f l5 = other; l5 *= layout;
	en::Matrix4f l6 = layout; l6 *= 2.0f;
	en::Matrix4f l7 = layout; l7 /= 2.0f;
	DOCTEST_CHECK(l1 == layout + en::Matrix4f::Identity());
	DOCTEST_CHECK(l2 == layout - en::Matrix4f::Identity());
	DOCTEST_CHECK(l3 == layout * en::Matrix4f::Identity());
	DOCTEST_CHECK(l4 == layout * other);
	DOCTEST_CHECK(l5 == other * layout);
	DOCTEST_CHECK(layout * 2.0f == en::Matrix4f(0, 2, 4, 6, 8, 10, 12, 14, 16, 18, 20, 22, 24, 26, 28, 30));
	DOCTEST_CHECK(layout / 2.0f == en::Matrix4f(0, 1 / 2.0f, 2 / 2.0f, 3 / 2.0f, 4 / 2.0f, 5 / 2.0f, 6 / 2.0f, 7 / 2.0f, 8 / 2.0f, 9 / 2.0f, 10 / 2.0f, 11 / 2.0f, 12 / 2.0f, 13 / 2.0f, 14 / 2.0f, 15 / 2.0f));
	DOCTEST_CHECK(l6 == en::Matrix4f(0, 2, 4, 6, 8, 10, 12, 14, 16, 18, 20, 22, 24, 26, 28, 30));
	DOCTEST_CHECK(l7 == en::Matrix4f(0, 1 / 2.0f, 2 / 2.0f, 3 / 2.0f, 4 / 2.0f, 5 / 2.0f, 6 / 2.0f, 7 / 2.0f, 8 / 2.0f, 9 / 2.0f, 10 / 2.0f, 11 / 2.0f, 12 / 2.0f, 13 / 2.0f, 14 / 2.0f, 15 / 2.0f));

	// Affine
	
	// Trace
	DOCTEST_CHECK(en::Matrix4f::Identity().GetTrace() == 4);
	DOCTEST_CHECK(layout.GetTrace() == 30);

	const en::Matrix4f weirdInversibleMatrix(1, 1, 1, -1, 1, 1, -1, 1, 1, -1, 1, 1, -1, 1, 1, 1);

	// Determinant
	DOCTEST_CHECK(en::Matrix4f::Identity().GetDeterminant() == 1);
	DOCTEST_CHECK(layout.GetDeterminant() == 0);
	DOCTEST_CHECK(weirdInversibleMatrix.GetDeterminant() == -16);
	DOCTEST_CHECK(en::Matrix4f::Translation(10.0f, 10.f, 10.0f).GetDeterminant() == 1);
	DOCTEST_CHECK(en::Matrix4f::Scale(2.0f).GetDeterminant() == 8);
	DOCTEST_CHECK(en::Matrix4f::RotationX(26.0f).GetDeterminant() == 1);
	DOCTEST_CHECK(en::Matrix4f::RotationY(26.0f).GetDeterminant() == 1);
	DOCTEST_CHECK(en::Matrix4f::RotationZ(26.0f).GetDeterminant() == 1);

	// Inverse
	en::Matrix4f invTest(2, 0, -1, 0, 0, 1, 0, 0, 1, 0, 1, 0, 0, 0, 0, 1);
	en::Matrix4f invResult(1 / 3.0f, 0, 1 / 3.0f, 0, 0, 1, 0, 0, -1 / 3.0f, 0, 2 / 3.0f, 0, 0, 0, 0, 1);
	DOCTEST_CHECK(invTest.Inversed() == invResult);
	DOCTEST_CHECK(invTest.Inverse() == invResult);
	DOCTEST_CHECK(en::Matrix4f::Equals(invResult.Inversed().Inversed(), invResult));
	//DOCTEST_CHECK(layout.Inversed().IsIdentity()); // Layout is not inversible
	DOCTEST_CHECK(weirdInversibleMatrix.Inversed() == weirdInversibleMatrix / 4);
	DOCTEST_CHECK(en::Matrix4f::Translation(10.0f, 10.f, 10.0f).Inversed() == en::Matrix4f::Translation(-10.0f, -10.0f, -10.0f));
	DOCTEST_CHECK(en::Matrix4f::Scale(2.0f).Inversed() == en::Matrix4f::Scale(0.5f));
	DOCTEST_CHECK(en::Matrix4f::RotationX(26.0f).Inversed() == en::Matrix4f::RotationX(-26.0f));
	DOCTEST_CHECK(en::Matrix4f::RotationY(26.0f).Inversed() == en::Matrix4f::RotationY(-26.0f));
	DOCTEST_CHECK(en::Matrix4f::RotationZ(26.0f).Inversed() == en::Matrix4f::RotationZ(-26.0f));
	
	// Transpose
	en::Matrix4f transposeTest(0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15);
	en::Matrix4f transposeResult(0, 4, 8, 12, 1, 5, 9, 13, 2, 6, 10, 14, 3, 7, 11, 15);
	DOCTEST_CHECK(transposeTest.Transposed() == transposeResult);
	DOCTEST_CHECK(transposeTest.Transpose() == transposeResult);
	DOCTEST_CHECK(transposeResult.Transposed().Transposed() == transposeResult);

	// TransformPoint / TransformDirection
	DOCTEST_CHECK(en::Vector3f::Equals(en::Matrix4f::RotationY(90.0f).TransformDirection(en::Vector3f::UnitX()), -en::Vector3f::UnitZ()));
	DOCTEST_CHECK(en::Vector3f::Equals(en::Matrix4f::RotationZ(90.0f).TransformDirection(en::Vector3f::UnitY()), -en::Vector3f::UnitX()));
	DOCTEST_CHECK(en::Vector3f::Equals(en::Matrix4f::RotationX(90.0f).TransformDirection(en::Vector3f::UnitZ()), -en::Vector3f::UnitY()));

	// Translation alone
	en::Matrix4f mTranslation = en::Matrix4f::Identity();
	mTranslation.SetColumn(3, en::Vector4f(1.0f, 2.0f, 3.0f, 1.0f));
	DOCTEST_CHECK(en::Matrix4f::Translation(1.0f, 2.0f, 3.0f) == mTranslation);
	DOCTEST_CHECK(en::Matrix4f::Translation(en::Vector3f(1.0f, 2.0f, 3.0f)) == mTranslation);
	DOCTEST_CHECK(en::Matrix4f::Translation(1.0f, 2.0f, 3.0f).GetTranslation() == en::Vector3f(1.0f, 2.0f, 3.0f));
	DOCTEST_CHECK(en::Matrix4f::Translation(en::Vector3f(1.0f, 2.0f, 3.0f)).GetTranslation() == en::Vector3f(1.0f, 2.0f, 3.0f));
	mTranslation.ApplyTranslation(en::Vector3f(10.0f, 20.0f, 30.0f));
	DOCTEST_CHECK(mTranslation.GetTranslation() == en::Vector3f(11, 22, 33));
	mTranslation.ApplyTranslation(-10.0f, -20.0f, -30.0f);
	DOCTEST_CHECK(mTranslation.GetTranslation() == en::Vector3f(1, 2, 3));
	mTranslation.SetTranslation(en::Vector3f(10, 20, 30));
	DOCTEST_CHECK(mTranslation.GetTranslation() == en::Vector3f(10, 20, 30));
	mTranslation.SetTranslation(100, 200, 300);
	DOCTEST_CHECK(mTranslation.GetTranslation() == en::Vector3f(100, 200, 300));

	// Scale alone
	en::Matrix4f mScale = en::Matrix4f::Diagonal(en::Vector4f(1.0f, 2.0f, 3.0f, 1.0f));
	en::Matrix4f mScaleUniform = en::Matrix4f::Diagonal(en::Vector4f(2.5f, 2.5f, 2.5f, 1.0f));
	DOCTEST_CHECK(en::Matrix4f::Scale(1.0f, 2.0f, 3.0f) == mScale);
	DOCTEST_CHECK(en::Matrix4f::Scale(en::Vector3f(1.0f, 2.0f, 3.0f)) == mScale);
	DOCTEST_CHECK(en::Matrix4f::Scale(2.5f) == mScaleUniform);
	DOCTEST_CHECK(en::Matrix4f::Scale(1.0f, 2.0f, 3.0f).GetScale() == en::Vector3f(1.0f, 2.0f, 3.0f));
	DOCTEST_CHECK(en::Matrix4f::Scale(en::Vector3f(1.0f, 2.0f, 3.0f)).GetScale() == en::Vector3f(1.0f, 2.0f, 3.0f));
	DOCTEST_CHECK(en::Matrix4f::Scale(2.5f).GetScale() == en::Vector3f(2.5f));
	DOCTEST_CHECK(!en::Matrix4f::Identity().IsScaled());
	DOCTEST_CHECK(en::Matrix4f::Identity().HasUniformScale());
	DOCTEST_CHECK(en::Matrix4f::Scale(2.0f).IsScaled());
	DOCTEST_CHECK(en::Matrix4f::Scale(2.0f).HasUniformScale());
	DOCTEST_CHECK(en::Matrix4f::Scale(1.0f, 2.0f, 3.0f).IsScaled());
	DOCTEST_CHECK(!en::Matrix4f::Scale(1.0f, 2.0f, 3.0f).HasUniformScale());
	mScale.ApplyScale(en::Vector3f(10.0f, 10.0f, 10.0f));
	DOCTEST_CHECK(mScale.GetScale() == en::Vector3f(10, 20, 30));
	mScale.ApplyScale(0.1f, 0.1f, 0.1f);
	DOCTEST_CHECK(mScale.GetScale() == en::Vector3f(1, 2, 3));
	mScale.SetScale(en::Vector3f(4, 5, 6));
	DOCTEST_CHECK(mScale.GetScale() == en::Vector3f(4, 5, 6));
	mScale.SetScale(7, 8, 9);
	DOCTEST_CHECK(mScale.GetScale() == en::Vector3f(7, 8, 9));

	// Rotation alone
	en::Matrix3f rotx26 = en::Matrix3f::RotationX(26.0f);
	en::Matrix3f roty26 = en::Matrix3f::RotationY(26.0f);
	en::Matrix3f rotz26 = en::Matrix3f::RotationZ(26.0f);
	DOCTEST_CHECK(en::Matrix4f::RotationAxis(26.0f, en::Vector3f::UnitX()).GetRotation() == rotx26);
	DOCTEST_CHECK(en::Matrix4f::RotationAxis(26.0f, 1, 0, 0).GetRotation() == rotx26);
	DOCTEST_CHECK(en::Matrix4f::RotationAxis(26.0f, en::Vector3f::UnitY()).GetRotation() == roty26);
	DOCTEST_CHECK(en::Matrix4f::RotationAxis(26.0f, 0, 1, 0).GetRotation() == roty26);
	DOCTEST_CHECK(en::Matrix4f::RotationAxis(26.0f, en::Vector3f::UnitZ()).GetRotation() == rotz26);
	DOCTEST_CHECK(en::Matrix4f::RotationAxis(26.0f, 0, 0, 1).GetRotation() == rotz26);
	DOCTEST_CHECK(en::Matrix4f::RotationX(26.0f).GetRotation() == rotx26);
	DOCTEST_CHECK(en::Matrix4f::RotationY(26.0f).GetRotation() == roty26);
	DOCTEST_CHECK(en::Matrix4f::RotationZ(26.0f).GetRotation() == rotz26);
	DOCTEST_CHECK(en::Matrix4f::Rotation(rotx26).GetRotation() == rotx26);
	DOCTEST_CHECK(en::Matrix4f::Rotation(roty26).GetRotation() == roty26);
	DOCTEST_CHECK(en::Matrix4f::Rotation(rotz26).GetRotation() == rotz26);
	// TODO : GLM update
	// TODO : ApplyRotation
	// TODO : SetRotation

	// Transformations
	const en::Vector3f vector(1.0f, 2.0f, 3.0f);
	const en::Matrix4f rotation = en::Matrix4f::Rotation(roty26); DOCTEST_CHECK(rotation.GetRotation() == roty26);
	const en::Matrix4f scale = en::Matrix4f::Scale(vector); DOCTEST_CHECK(scale.GetScale() == vector);
	const en::Matrix4f translation = en::Matrix4f::Translation(vector); DOCTEST_CHECK(translation.GetTranslation() == vector);

	// Rotation+Scale
	en::Matrix4f rotationWithScale = rotation * scale;
	DOCTEST_CHECK(en::Vector3f::Equals(rotationWithScale.GetScale(), vector));
	DOCTEST_CHECK(en::Matrix3f::Equals(rotationWithScale.GetRotation(), roty26));
	DOCTEST_CHECK(en::Vector3f::Equals(rotationWithScale.GetTranslation(), en::Vector3f()));
	rotationWithScale.SetScale(3.0f);
	DOCTEST_CHECK(en::Vector3f::Equals(rotationWithScale.GetScale(), en::Vector3f(3.0f)));
	DOCTEST_CHECK(en::Matrix3f::Equals(rotationWithScale.GetRotation(), roty26));
	DOCTEST_CHECK(en::Vector3f::Equals(rotationWithScale.GetTranslation(), en::Vector3f()));
	rotationWithScale.SetRotation(rotz26);
	DOCTEST_CHECK(en::Vector3f::Equals(rotationWithScale.GetScale(), en::Vector3f(3.0f)));
	DOCTEST_CHECK(en::Matrix3f::Equals(rotationWithScale.GetRotation(), rotz26));
	DOCTEST_CHECK(en::Vector3f::Equals(rotationWithScale.GetTranslation(), en::Vector3f()));

	// TODO : Rotation+Translation
	// TODO : GLM update

	// TODO : Translation+Scale
	// TODO : GLM update

	const en::Matrix4f transform = translation * rotation * scale;
	const en::Matrix4f transform2 = en::Matrix4f::Transform(vector, roty26, vector);
	DOCTEST_CHECK(transform2 == transform);

	// TODO : GLM update
	/*
	en::Matrix4f at = transform;
	at.SetScale(3.0f);
	DOCTEST_CHECK(at.GetScale() == en::Vector3f(3.0f));
	DOCTEST_CHECK(at.GetRotation() == roty26);
	DOCTEST_CHECK(at.GetTranslation() == vector);

	en::Matrix4f bt = transform;
	bt.SetRotation(rotz26);
	DOCTEST_CHECK(bt.GetScale() == vector);
	DOCTEST_CHECK(bt.GetRotation() == rotz26);
	DOCTEST_CHECK(bt.GetTranslation() == vector);

	en::Matrix4f m;
	m.SetScale(vector);
	m.SetRotation(roty26);
	DOCTEST_CHECK(m.GetScale() == vector);
	m.ApplyScale(2.0f);
	DOCTEST_CHECK(m.GetScale() == 2.0f * vector);
	m.ApplyScale(vector);
	DOCTEST_CHECK(m.GetScale() != en::Vector3f(1.0f * 2.0f * 1.0f, 2.0f * 2.0f * 2.0f, 3.0f * 2.0f * 3.0f)); // Non-uniform scale applied after rotation

	en::Vector3f tScale = en::Vector3f(2.0f, 3.0f, 4.0f);
	en::Matrix4f t = transform;
	t.ApplyScale(tScale);
	DOCTEST_CHECK(t.GetTranslation() == tScale * vector);
	DOCTEST_CHECK(t.GetScale() != tScale * vector); // Non-uniform scale applied after rotation
	DOCTEST_CHECK(t.GetRotation() != roty26); // Non-uniform scale applied after rotation
	*/

	// TODO : GLM update
	// TODO : Perspective
	// TODO : Orthographic
	// TODO : LookAt
}

