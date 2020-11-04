#pragma once

#include <Enlivengine/Config.hpp>

#ifdef ENLIVE_MODULE_MATH

#include <Enlivengine/Math/Vector3.hpp>
#include <Enlivengine/Math/Vector4.hpp>

namespace en
{

template <typename T>
class Quaternion
{
public:
	constexpr Quaternion() : v(), s(T(0)) {}
	constexpr Quaternion(const Quaternion<T>& q) : v(q.v), s(q.s) {}
	template <typename U>
	constexpr Quaternion(const Quaternion<U>& q) : v(q.v), s(static_cast<T>(q.s)) {}
	constexpr Quaternion(const T* a) : v(a), s(a[3]) {}
	constexpr Quaternion(const Vector4<T>& v) : v(v.x, v.y, v.z), q(v.w) {}
	constexpr Quaternion(const T& x, const T& y, const T& z, const T& w) : v(x, y, z, w), s(w) {}
	constexpr Quaternion(const Vector3<T>& v, const T& s) : v(v), s(s) {}
	constexpr Quaternion(const Vector3<T>& eulerAngles) { Set(eulerAngles); }
	inline Quaternion(const T& angle, const Vector3<T>& axis) { Set(angle, axis); }
	inline Quaternion(const Matrix3<T>& matrix) { Set(matrix); }

	constexpr Quaternion<T>& Set(const Quaternion<T>& q) { v = q.v; s = q.s; return *this; }
	template <typename U>
	constexpr Quaternion<T>& Set(const Quaternion<U>& q) { v.Set(q.v); s = static_cast<T>(q.s); return *this; }
	constexpr Quaternion<T>& Set(const T* a) { v.Set(a); q = a[3]; return *this; }
	constexpr Quaternion<T>& Set(const Vector4<T>& v) { this->v.Set(v.x, v.y, v.z); s = v.w; return *this; }
	constexpr Quaternion<T>& Set(const T& x, const T& y, const T& z, const T& w) { v.Set(x, y, z); s = w; return *this; }
	constexpr Quaternion<T>& Set(const Vector3<T>& v, const T& s) { this->v.Set(v); this->s = s; return *this; }
	constexpr Quaternion<T>& Set(const Vector3<T>& eulerAngles)
	{
		Vector3<T> halfAngles(T(0.5) * eulerAngles);
		const T sx = Math::Sin(halfAngles.x);
		const T cx = Math::Cos(halfAngles.x);
		const T sy = Math::Sin(halfAngles.y);
		const T cy = Math::Cos(halfAngles.y);
		const T sz = Math::Sin(halfAngles.z);
		const T cz = Math::Cos(halfAngles.z);
		v.x = sx * cy * cz - cx * sy * sz;
		v.y = cx * sy * cz + sx * cy * sz;
		v.z = cx * cy * sz - sx * sy * cz;
		s = cx * cy * cz + sx * sy * sz;
		return *this;
	}
	inline Quaternion<T>& Set(const T& angle, const Vector3<T>& axis)
	{
		const T halfAngle = angle * T(0.5);
		v.Set(axis.Normalized() * Math::Sin(halfAngle));
		s = Math::Cos(halfAngle);
		return *this;
	}
	inline Quaternion<T>& Set(const Matrix3<T>& matrix)
	{
		const T trace = matrix.GetTrace();
		if (trace > 0)
		{
			const T t = Math::FastSqrt(trace + 1) * 2;
			const T overT = 1 / t;
			v.x = (matrix[5] - matrix[7]) * overT;
			v.y = (matrix[6] - matrix[2]) * overT;
			v.z = (matrix[1] - matrix[3]) * overT;
			s = t * T(0.25);
			return *this;
		}
		else if (matrix[0] > matrix[4] && matrix[0] > matrix[8])
		{
			const T t = Math::FastSqrt(matrix[0] - matrix[4] - matrix[8] + 1) * 2;
			const T overT = 1 / t;
			v.x = t * T(0.25);
			v.y = (matrix[3] + matrix[1]) * overT;
			v.z = (matrix[6] + matrix[2]) * overT;
			s = (matrix[5] - matrix[7]) * overT;
			return *this;
		}
		else if (matrix[4] > matrix[8])
		{
			const T t = Math::FastSqrt(matrix[4] - matrix[0] - matrix[8] + 1) * 2;
			const T overT = 1 / t;
			v.x = (matrix[3] + matrix[1]) * overT;
			v.y = t * T(0.25);
			v.z = (matrix[5] + matrix[7]) * overT;
			s = (matrix[6] - matrix[2]) * overT;
			return *this;
		}
		else
		{
			const T t = Math::FastSqrt(matrix[8] - matrix[0] - matrix[4] + 1) * 2;
			const T overT = 1 / t;
			v.x = (matrix[6] + matrix[2]) * overT;
			v.y = (matrix[5] + matrix[7]) * overT;
			v.z = t * T(0.25);
			s = (matrix[1] - matrix[3]) * overT;
			return *this;
		}
	}

	constexpr T& operator()(U32 i) { if (i == 0) return v.x; else if (i == 1) return v.y; else if (i == 2) return v.z; return s; }
	constexpr const T& operator()(U32 i) const { if (i == 0) return v.x; else if (i == 1) return v.y; else if (i == 2) return v.z; return s; }
	constexpr T& operator[](U32 i) { if (i == 0) return v.x; else if (i == 1) return v.y; else if (i == 2) return v.z; return s; }
	constexpr const T& operator[](U32 i) const { if (i == 0) return v.x; else if (i == 1) return v.y; else if (i == 2) return v.z; return s; }

	constexpr Quaternion<T>& operator=(const Quaternion<T>& q) { v = q.v; s = q.s; return *this; }

	constexpr Quaternion<T> operator*(const Quaternion<T>& q) const { return Quaternion<T>(s * q.v + q.s * v + v.CrossProduct(q.v), s * q.s - v.DotProduct(q.v)); }
	constexpr Quaternion<T>& operator*=(const Quaternion<T>& q)
	{
		Quaternion<T> p(*this);
		v = p.s * q.v;
		v += q.s * p.v;
		v += p.v.CrossProduct(q.v);
		s = p.s * q.s - p.v.DotProduct(q.v);
		return *this;
	}
	inline Quaternion<T> operator*(const T& s) const
	{
		T angle;
		Vector3<T> axis;
		ToAngleAxis(angle, axis);
		angle *= s;
		angle *= T(0.5);
		return Quaternion<T>(axis.Normalized() * Math::Sin(angle), Math::Cos(angle));
	}
	inline Quaternion<T>& operator*=(const T& s)
	{
		T angle;
		Vector3<T> axis;
		ToAngleAxis(angle, axis);
		angle *= s;
		angle *= T(0.5);
		v.Set(axis.Normalized() * Math::Sin(angle));
		s = Math::Cos(angle);
		return *this;
	}

	constexpr Vector3<T> Rotate(const Vector3<T>& vector) const
	{
		T ss = s + s;
		return ss * (v.CrossProduct(vector)) + (ss * s - T(1)) * vector + T(2) * (v.DotProduct(vector)) * vector;
	}

	constexpr bool operator==(const Quaternion<T>& q) const { return v == other.v && s == other.s; }
	constexpr bool operator!=(const Quaternion<T>& q) const { return !operator==(other); }

	constexpr Quaternion<T>& Conjugate() { v *= T(-1); return *this; }
	constexpr Quaternion<T> Conjugated() const { return Quaternion<T>(-v, s); }

	constexpr Quaternion<T>& Inverse()
	{
		Conjugate();
		const T squaredLength = GetSquaredLength();
		if (!Math::Equals(squaredLength, T(1)))
		{
			v /= squaredLength;
			s /= squaredLength;
		}
		return *this;
	}
	constexpr Quaternion<T> Inversed() const { return Quaternion<T>(*this).Inverse(); }

	constexpr T DotProduct(const Quaternion<T>& q) const { return v.DotProduct(q.v) + s * q.s; }
	constexpr T GetSquaredLength() const { return DotProduct(*this); }
	inline T GetLength() const { return Math::FastSqrt(GetSquaredLength()); }
	inline Quaternion<T>& Normalize(T* oldLength = nullptr)
	{
		const T currentLength = GetLength();
		if (oldLength != nullptr)
		{
			*oldLength = currentLength;
		}
		const T factor = T(1) / currentLength;
		v *= factor;
		s *= factor;
		return *this;
	}
	inline Quaternion<T> Normalized(T* oldLength = nullptr) const { return Quaternion<T>(*this).Normalize(oldLength); }
	inline Quaternion<T>& EnsureNormalized()
	{
		if (!Math::Equals(GetSquaredLength(), T(1)))
		{
			return Normalize();
		}
		else
		{
			return *this;
		}
	}

	inline void ToEulerAngles(Vector3<T>& vector) const
	{
		const Matrix3<T> matrix(ToMatrix3());
		const T cos2 = matrix[0] * matrix[0] + matrix[1] * matrix[1];
		if (cos2 < T(0.000001))
		{
			vector.set(0, (matrix[2] < 0) ? Math::HalfPi : -Math::HalfPi, -Math::Atan2(matrix[4], matrix[3]));
		}
		else
		{
			vector.set(Math::Atan2(matrix[8], matrix[5]), Math::Atan2(Math::FastSqrt(cos2), -matrix[2]), Math::Atan2(matrix[0], matrix[1]));
		}
	}
	constexpr void ToAngleAxis(T& angle, Vector3<T>& axis) const
	{
		axis = (s > T(0)) ? v : -v;
		angle = T(2) * Math::Atan(Math::Abs(s));
	}
	constexpr void ToMatrix3(Matrix3<T>& matrix) const
	{
		const T x2 = v.x * v.x; const T y2 = v.y * v.y; const T z2 = v.z * v.z;
		const T xs = v.x * s;   const T ys = v.y * s;   const T zs = v.z * s;
		const T xy = v.x * v.y; const T xz = v.x * v.z; const T yz = v.y * v.z;
		matrix.Set(1 - 2 * (y2 + z2), 2 * (xy + zs), 2 * (xz - ys),
			2 * (xy - zs), 1 - 2 * (x2 + z2), 2 * (xs + yz),
			2 * (ys + xz), 2 * (yz - xs), 1 - 2 * (x2 + y2));
	}

	static constexpr Quaternion<T> Slerp(const Quaternion<T>& start, const Quaternion<T>& end, const T& percent)
	{
		const T dot = start.DotProduct(end);
		if (dot > T(0.999999))
		{
			const T mp = T(1) - percent;
			return Quaternion<T>(start.v * mp + end.v * percent, start.s * mp + end.s * percent);
		}
		return Quaternion<T>(start * ((start.Inversed() * end) * percent));
	}

	static inline Quaternion<T> RotateFromTo(const Vector3<T>& start, const Vector3<T>& end, const Vector3<T>& axis = ENLIVE_DEFAULT_UP)
	{
		const Vector3<T> v1 = start.Normalized();
		const Vector3<T> v2 = end.Normalized();
		const T dot = v1.DotProduct(v2);
		if (dot >= T(0.99999847691))
		{
			return Quaternion<T>::Identity();
		}
		if (dot <= T(-0.99999847691))
		{
			return Quaternion<T>(0, axis);
		}
		const Vector3<T> cross = v1.CrossProduct(v2);
		return Quaternion<T>(cross, dot + T(1)).Normalize();
	}

	static constexpr Quaternion<T> Identity() { return Quaternion<T>(Vector3f(), T(1)); }

	Vector3<T> v;
	T s;
};

template <typename T> constexpr Quaternion<T> operator*(const T& s, const Quaternion<T>& quaternion) { return quaternion * s; }

typedef Quaternion<F32> Quaternionf;

typedef Quaternionf quat; // GLSL-like

} // namespace en

#endif // ENLIVE_MODULE_MATH