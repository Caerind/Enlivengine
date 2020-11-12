#include <Enlivengine/Math/Quaternion.hpp>

#include <doctest/doctest.h>

DOCTEST_TEST_CASE("Quaternion")
{
	DOCTEST_CHECK(en::Quaternionf::Identity() == en::Quaternionf(0.0f, 0.0f, 0.0f, 1.0f));

	// Axis-angle => Quaternion
	DOCTEST_CHECK(en::Quaternionf(0.0f, en::Vector3f(0.0f, 0.0f, 0.0f)) == en::Quaternionf::Identity());
	DOCTEST_CHECK(en::Quaternionf(0.0f, en::Vector3f(1.0f, 0.0f, 0.0f)) == en::Quaternionf::Identity());
	DOCTEST_CHECK(en::Quaternionf(0.0f, en::Vector3f(0.0f, 1.0f, 0.0f)) == en::Quaternionf::Identity());
	DOCTEST_CHECK(en::Quaternionf(0.0f, en::Vector3f(0.0f, 0.0f, 1.0f)) == en::Quaternionf::Identity());
	DOCTEST_CHECK(en::Quaternionf(0.0f, en::Vector3f(1.0f, 1.0f, 1.0f)) == en::Quaternionf::Identity());
	DOCTEST_CHECK(en::Quaternionf(36.0f, en::Vector3f(1.0f, 0.0f, 0.0f)) == en::Quaternionf(0.309017, 0, 0, 0.9510565));
	DOCTEST_CHECK(en::Quaternionf(36.0f, en::Vector3f(0.0f, 1.0f, 0.0f)) == en::Quaternionf(0, 0.309017, 0, 0.9510565));
	DOCTEST_CHECK(en::Quaternionf(36.0f, en::Vector3f(0.0f, 0.0f, 1.0f)) == en::Quaternionf(0, 0, 0.309017, 0.9510565));
	DOCTEST_CHECK(en::Quaternionf(36.0f, en::Vector3f(1.0f, 1.0f, 0.0f)) == en::Quaternionf(0.218508, 0.218508, 0, 0.9510565));
	DOCTEST_CHECK(en::Quaternionf(36.0f, en::Vector3f(0.0f, 1.0f, 1.0f)) == en::Quaternionf(0, 0.218508, 0.218508, 0.9510565));
	DOCTEST_CHECK(en::Quaternionf(36.0f, en::Vector3f(-1.0f, -1.0f, 0.0f)) == en::Quaternionf(-0.218508, -0.218508, 0, 0.9510565));
	DOCTEST_CHECK(en::Quaternionf(36.0f, en::Vector3f(0.0f, -1.0f, -1.0f)) == en::Quaternionf(0, -0.218508, -0.218508, 0.9510565));
	DOCTEST_CHECK(en::Quaternionf(36.0f, en::Vector3f(36.0f, -108.0f, 190.0f)) == en::Quaternionf(0.0502251, -0.1506754, 0.2650771, 0.9510565));
	DOCTEST_CHECK(en::Quaternionf(91.0f, en::Vector3f(-1.0f, -2.0f, -3.0f)) == en::Quaternionf(-0.1906242, -0.3812484, -0.5718726, 0.7009093));
	DOCTEST_CHECK(en::Quaternionf(-36.0f, en::Vector3f(-1.0f, 2.0f, -3.0f)) == en::Quaternionf(0.0825883, -0.1651765, 0.2477648, 0.9510565));
	// Quaternion => Angle-axis
	en::F32 angle; en::Vector3f axis;
	en::Quaternionf::Identity().ToAngleAxis(angle, axis);
	DOCTEST_CHECK(angle == 0.0f);
	en::Quaternionf(0.309017, 0, 0, 0.9510565).ToAngleAxis(angle, axis);
	DOCTEST_CHECK(en::Math::Equals(angle, 36.0f)); DOCTEST_CHECK(axis == en::Vector3f(1.0f, 0.0f, 0.0f));
	en::Quaternionf(0, 0.309017, 0, 0.9510565).ToAngleAxis(angle, axis);
	DOCTEST_CHECK(en::Math::Equals(angle, 36.0f)); DOCTEST_CHECK(axis == en::Vector3f(0.0f, 1.0f, 0.0f));
	en::Quaternionf(0, 0, 0.309017, 0.9510565).ToAngleAxis(angle, axis);
	DOCTEST_CHECK(en::Math::Equals(angle, 36.0f)); DOCTEST_CHECK(axis == en::Vector3f(0.0f, 0.0f, 1.0f));
	en::Quaternionf(0.218508, 0.218508, 0, 0.9510565).ToAngleAxis(angle, axis);
	DOCTEST_CHECK(en::Math::Equals(angle, 36.0f)); DOCTEST_CHECK(axis == en::Vector3f(1.0f, 1.0f, 0.0f).Normalized());
	en::Quaternionf(0, 0.218508, 0.218508, 0.9510565).ToAngleAxis(angle, axis);
	DOCTEST_CHECK(en::Math::Equals(angle, 36.0f)); DOCTEST_CHECK(axis == en::Vector3f(0.0f, 1.0f, 1.0f).Normalized());
	en::Quaternionf(-0.218508, -0.218508, 0, 0.9510565).ToAngleAxis(angle, axis);
	DOCTEST_CHECK(en::Math::Equals(angle, 36.0f)); DOCTEST_CHECK(axis == en::Vector3f(-1.0f, -1.0f, 0.0f).Normalized());
	en::Quaternionf(0, -0.218508, -0.218508, 0.9510565).ToAngleAxis(angle, axis);
	DOCTEST_CHECK(en::Math::Equals(angle, 36.0f)); DOCTEST_CHECK(axis == en::Vector3f(0.0f, -1.0f, -1.0f).Normalized());
	en::Quaternionf(0.0502251, -0.1506754, 0.2650771, 0.9510565).ToAngleAxis(angle, axis);
	DOCTEST_CHECK(en::Math::Equals(angle, 36.0f)); DOCTEST_CHECK(axis == en::Vector3f(36.0f, -108.0f, 190.0f).Normalized());

	// Matrix => Quaternion
	DOCTEST_CHECK(en::Quaternionf(en::Matrix3f::Identity()) == en::Quaternionf::Identity());
	DOCTEST_CHECK(en::Quaternionf(en::Matrix3f::RotationX(36.0f)) == en::Quaternionf(36.0f, en::Vector3f(1.0f, 0.0f, 0.0f)));
	DOCTEST_CHECK(en::Quaternionf(en::Matrix3f::RotationY(36.0f)) == en::Quaternionf(36.0f, en::Vector3f(0.0f, 1.0f, 0.0f)));
	DOCTEST_CHECK(en::Quaternionf(en::Matrix3f::RotationZ(36.0f)) == en::Quaternionf(36.0f, en::Vector3f(0.0f, 0.0f, 1.0f)));
	// TODO : This is not tested enough, we should test more matrix weird angles (negative, +180, +360, xyz) (gimbal lock)
	// Quaternion => Matrix
	DOCTEST_CHECK(en::Quaternionf::Identity().ToMatrix3() == en::Matrix3f::Identity());
	DOCTEST_CHECK(en::Quaternionf(36.0f, en::Vector3f(1.0f, 0.0f, 0.0f)).ToMatrix3() == en::Matrix3f(1.0000000, 0.0000000, 0.0000000, 0.0000000, 0.8090170, -0.5877852, 0.0000000, 0.5877852, 0.8090170));
	DOCTEST_CHECK(en::Quaternionf(36.0f, en::Vector3f(0.0f, 1.0f, 0.0f)).ToMatrix3() == en::Matrix3f(0.8090170, 0.0000000, 0.5877852, 0.0000000, 1.0000000, 0.0000000, -0.5877852, 0.0000000, 0.8090170));
	DOCTEST_CHECK(en::Quaternionf(36.0f, en::Vector3f(0.0f, 0.0f, 1.0f)).ToMatrix3() == en::Matrix3f(0.8090170, -0.5877852, 0.0000000, 0.5877852, 0.8090170, 0.0000000,	0.0000000, 0.0000000, 1.0000000));
	DOCTEST_CHECK(en::Quaternionf(36.0f, en::Vector3f(1.0f, 1.0f, 0.0f)).ToMatrix3() == en::Matrix3f(0.9045085, 0.0954915, 0.4156269, 0.0954915, 0.9045085, -0.4156269, -0.4156269, 0.4156269, 0.8090170));
	DOCTEST_CHECK(en::Quaternionf(36.0f, en::Vector3f(0.0f, 1.0f, 1.0f)).ToMatrix3() == en::Matrix3f(0.8090170, -0.4156269, 0.4156269, 0.4156269, 0.9045085, 0.0954915, -0.4156269, 0.0954915, 0.9045085));
	DOCTEST_CHECK(en::Quaternionf(36.0f, en::Vector3f(-1.0f, -1.0f, 0.0f)).ToMatrix3() == en::Matrix3f(0.9045085, 0.0954915, -0.4156269, 0.0954915, 0.9045085, 0.4156269, 0.4156269, -0.4156269, 0.8090170));
	DOCTEST_CHECK(en::Quaternionf(36.0f, en::Vector3f(0.0f, -1.0f, -1.0f)).ToMatrix3() == en::Matrix3f(0.8090170, 0.4156269, -0.4156269, -0.4156269, 0.9045085, 0.0954915, 0.4156269, 0.0954915, 0.9045085));
	DOCTEST_CHECK(en::Quaternionf(36.0f, en::Vector3f(36.0f, -108.0f, 190.0f)).ToMatrix3() == en::Matrix3f(0.8140621, -0.5193419, -0.2599746, 0.4890712, 0.8544232, -0.1754151, 0.3132287, 0.0156527, 0.9495487));
	DOCTEST_CHECK(en::Quaternionf(36.0f, en::Vector3f(36.0f, -108.0f, 190.0f)).ToMatrix3() == en::Matrix3f(0.8140621, -0.5193419, -0.2599746, 0.4890712, 0.8544232, -0.1754151, 0.3132287, 0.0156527, 0.9495487));
	DOCTEST_CHECK(en::Quaternionf(91.0f, en::Vector3f(-1.0f, -2.0f, -3.0f)).ToMatrix3() == en::Matrix3f(0.0552228, 0.9470119, -0.3164155, -0.6563113, 0.2732483, 0.7032716, 0.7524666, 0.1688305, 0.6366242));
	DOCTEST_CHECK(en::Quaternionf(-36.0f, en::Vector3f(-1.0f, 2.0f, -3.0f)).ToMatrix3() == en::Matrix3f(0.8226587, -0.4985599, -0.2732595, 0.4439934, 0.8635836, -0.2389421, 0.3551094, 0.0752424, 0.9317918));

	// EulerAngles => Quaternion
	DOCTEST_CHECK(en::Quaternionf(en::Vector3f(0.0f, 0.0f, 0.0f)) == en::Quaternionf::Identity());
	DOCTEST_CHECK(en::Quaternionf(en::Vector3f(36.0f, 0.0f, 0.0f)) == en::Quaternionf(0.309017f, 0.0f, 0.0f, 0.9510565f));
	DOCTEST_CHECK(en::Quaternionf(en::Vector3f(0.0f, 36.0f, 0.0f)) == en::Quaternionf(0.0f, 0.309017f, 0.0f, 0.9510565f));
	DOCTEST_CHECK(en::Quaternionf(en::Vector3f(0.0f, 0.0f, 36.0f)) == en::Quaternionf(0.0f, 0.0f, 0.309017f, 0.9510565f));
	DOCTEST_CHECK(en::Quaternionf(en::Vector3f(36.0f, 36.0f, 0.0f)) == en::Quaternionf(0.2938926f, 0.2938926f, 0.0954915f, 0.9045085f));
	DOCTEST_CHECK(en::Quaternionf(en::Vector3f(0.0f, 36.0f, 36.0f)) == en::Quaternionf(0.0954915f, 0.2938926f, 0.2938926f, 0.9045085f));
	DOCTEST_CHECK(en::Quaternionf(en::Vector3f(-36.0f, -36.0f, 0.0f)) == en::Quaternionf(-0.2938926f, -0.2938926f, 0.0954915f, 0.9045085f));
	DOCTEST_CHECK(en::Quaternionf(en::Vector3f(0.0f, -36.0f, -36.0f)) == en::Quaternionf(0.0954915f, -0.2938926f, -0.2938926f, 0.9045085f));
	DOCTEST_CHECK(en::Quaternionf(en::Vector3f(36.0f, -108.0f, 190.0f)) == en::Quaternionf(-0.7823236f, -0.113885f, 0.5786787f, 0.2003271f));
	// Quaternion => EulerAngles
	DOCTEST_CHECK(en::Vector3f(0.0f, 0.0f, 0.0f) == en::Quaternionf::Identity().ToEulerAngles());
	DOCTEST_CHECK(en::Vector3f(36.0f, 0.0f, 0.0f) == en::Quaternionf(0.309017f, 0.0f, 0.0f, 0.9510565f).ToEulerAngles());
	DOCTEST_CHECK(en::Vector3f(0.0f, 36.0f, 0.0f) == en::Quaternionf(0.0f, 0.309017f, 0.0f, 0.9510565f).ToEulerAngles());
	DOCTEST_CHECK(en::Vector3f(0.0f, 0.0f, 36.0f) == en::Quaternionf(0.0f, 0.0f, 0.309017f, 0.9510565f).ToEulerAngles());
	DOCTEST_CHECK(en::Vector3f(0.0f, 36.0f, 36.0f) == en::Quaternionf(0.0954915f, 0.2938926f, 0.2938926f, 0.9045085f).ToEulerAngles());
	DOCTEST_CHECK(en::Vector3f(-36.0f, -36.0f, 0.0f) == en::Quaternionf(-0.2938926f, -0.2938926f, 0.0954915f, 0.9045085f).ToEulerAngles());
	DOCTEST_CHECK(en::Vector3f(0.0f, -36.0f, -36.0f) == en::Quaternionf(0.0954915f, -0.2938926f, -0.2938926f, 0.9045085f).ToEulerAngles());
	// TODO : This is not tested enough, we should test more matrix weird angles (negative, +180, +360, xyz) (gimbal lock)

	// Inverse
	for (en::F32 i = 0.0f; i < 360.0f; i += 10.0f)
	{
		DOCTEST_CHECK(en::Quaternionf(en::Matrix3f::RotationX(36.0f).Inversed()) == en::Quaternionf(en::Matrix3f::RotationX(36.0f)).Inversed());
		DOCTEST_CHECK(en::Quaternionf(en::Matrix3f::RotationY(36.0f).Inversed()) == en::Quaternionf(en::Matrix3f::RotationY(36.0f)).Inversed());
		DOCTEST_CHECK(en::Quaternionf(en::Matrix3f::RotationZ(36.0f).Inversed()) == en::Quaternionf(en::Matrix3f::RotationZ(36.0f)).Inversed());
	}
}