#pragma once

#include <Enlivengine/Config.hpp>

#ifdef ENLIVE_MODULE_MATH

#include <Enlivengine/Math/Vector3.hpp>
#include <Enlivengine/Math/Vector4.hpp>
#include <Enlivengine/Math/Matrix3.hpp>

namespace en
{

template <typename T>
class Quaternion
{
public:
	constexpr Quaternion() : v(), s(T(1)) {}
	constexpr Quaternion(const Quaternion<T>& q) : v(q.v), s(q.s) {}
	template <typename U>
	constexpr Quaternion(const Quaternion<U>& q) : v(q.v), s(static_cast<T>(q.s)) {}
	constexpr Quaternion(const Vector4<T>& v) : v(v.x, v.y, v.z), q(v.w) {}
	constexpr Quaternion(const T& x, const T& y, const T& z, const T& w) : v(x, y, z), s(w) {}
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
		const Vector3<T> halfAngles(T(0.5) * eulerAngles);
		const T sx = Math::Sin(halfAngles.x);
		const T cx = Math::Cos(halfAngles.x);
		const T sy = Math::Sin(halfAngles.y);
		const T cy = Math::Cos(halfAngles.y);
		const T sz = Math::Sin(halfAngles.z);
		const T cz = Math::Cos(halfAngles.z);
		v.x = sx * cy * cz + cx * sy * sz;
		v.y = cx * sy * cz - sx * cy * sz;
		v.z = cx * cy * sz + sx * sy * cz;
		s = cx * cy * cz - sx * sy * sz;
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
			v.x = (matrix[7] - matrix[5]) * overT;
			v.y = (matrix[2] - matrix[6]) * overT;
			v.z = (matrix[3] - matrix[1]) * overT;
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
			s = (matrix[7] - matrix[5]) * overT;
			return *this;
		}
		else if (matrix[4] > matrix[8])
		{
			const T t = Math::FastSqrt(matrix[4] - matrix[0] - matrix[8] + 1) * 2;
			const T overT = 1 / t;
			v.x = (matrix[3] + matrix[1]) * overT;
			v.y = t * T(0.25);
			v.z = (matrix[5] + matrix[7]) * overT;
			s = (matrix[2] - matrix[6]) * overT;
			return *this;
		}
		else
		{
			const T t = Math::FastSqrt(matrix[8] - matrix[0] - matrix[4] + 1) * 2;
			const T overT = 1 / t;
			v.x = (matrix[6] + matrix[2]) * overT;
			v.y = (matrix[5] + matrix[7]) * overT;
			v.z = t * T(0.25);
			s = (matrix[3] - matrix[1]) * overT;
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

	constexpr bool operator==(const Quaternion<T>& other) const { return v == other.v && Math::Equals(s, other.s); }
	constexpr bool operator!=(const Quaternion<T>& other) const { return !operator==(other); }

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
		Matrix3<T> m;
		ToMatrix3(m);
		vector.y = Math::Asin(Math::Clamp(m[2], T(-1), T(1)));
		if (Math::Abs(m[2]) < 0.9999) 
		{
			vector.x = Math::Atan2(m[8], -m[5]);
			vector.z = Math::Atan2(m[0], -m[1]);
		}
		else 
		{
			vector.x = Math::Atan2(m[4], m[7]);
			vector.z = 0;
		}
	}
	inline Vector3<T> ToEulerAngles() const
	{
		Vector3<T> eulerAngles;
		ToEulerAngles(eulerAngles);
		return eulerAngles;
	}
	constexpr void ToAngleAxis(T& angle, Vector3<T>& axis) const
	{
		angle = 2 * Math::Acos(s);
		const T t = Math::FastSqrt(1 - s * s);
		axis = v;
		if (t > 0.001) 
		{
			axis /= t;
		}
	}
	constexpr void ToMatrix3(Matrix3<T>& matrix) const
	{
		const T x2 = v.x * v.x; const T y2 = v.y * v.y; const T z2 = v.z * v.z;
		const T xs = v.x * s;   const T ys = v.y * s;   const T zs = v.z * s;
		const T xy = v.x * v.y; const T xz = v.x * v.z; const T yz = v.y * v.z;
		matrix.Set(1 - 2 * (y2 + z2), 2 * (xy - zs), 2 * (xz + ys),
			2 * (xy + zs), 1 - 2 * (x2 + z2), 2 * (yz - xs),
			2 * (xz - ys), 2 * (yz + xs), 1 - 2 * (x2 + y2));
	}
	constexpr Matrix3<T> ToMatrix3() const
	{
		Matrix3<T> m;
		ToMatrix3(m);
		return m;
	}

	constexpr Vector3f GetForward() const { return Rotate(ENLIVE_DEFAULT_FORWARD); }
	constexpr Vector3f GetBackward() const { return Rotate(ENLIVE_DEFAULT_BACKWARD); }
	constexpr Vector3f GetUp() const { return Rotate(ENLIVE_DEFAULT_UP); }
	constexpr Vector3f GetDown() const { return Rotate(ENLIVE_DEFAULT_DOWN); }
	constexpr Vector3f GetLeft() const { return Rotate(ENLIVE_DEFAULT_LEFT); }
	constexpr Vector3f GetRight() const { return Rotate(ENLIVE_DEFAULT_RIGHT); }

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

	static constexpr Quaternion<T> Identity() { return Quaternion<T>(Vector3f::Zero(), T(1)); }

	Vector3<T> v;
	T s;
};

template <typename T> constexpr Quaternion<T> operator*(const T& s, const Quaternion<T>& quaternion) { return quaternion * s; }

typedef Quaternion<F32> Quaternionf;

typedef Quaternionf quat; // GLSL-like

} // namespace en

#endif // ENLIVE_MODULE_MATH