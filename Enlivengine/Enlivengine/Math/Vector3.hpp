#pragma once

#include <Enlivengine/Config.hpp>

#ifdef ENLIVE_MODULE_MATH

#include <Enlivengine/Math/Vector2.hpp>

// TODO : Constexpr Sqrt => GetLength, SetLength, Normalize, Normalized

namespace en
{

template <typename T>
class Vector3
{
public:
	static constexpr I32 Dim = 3;

	constexpr Vector3() : x(T(0)), y(T(0)), z(T(0)) {}
	constexpr Vector3(const Vector3<T>& v) : x(v.x), y(v.y), z(v.z) {}
	template <typename U>
	constexpr Vector3(const Vector3<U>& v) : x(static_cast<T>(v.x)), y(static_cast<T>(v.y)), z(static_cast<T>(v.z)) {}
	constexpr Vector3(const T& s) : x(s), y(s), z(s) {}
	constexpr Vector3(const T& s1, const T& s2, const T& s3) : x(s1), y(s2), z(s3) {}
	constexpr Vector3(const Vector2<T>& v12, const T& s3) : x(v12.x), y(v12.y), z(s3) {}
	constexpr Vector3(const T& s1, const Vector2<T>& v23) : x(s1), y(v23.y), z(v23.y) {}
	~Vector3() = default;

	constexpr Vector3<T>& Set(const Vector3<T> & v) { x = v.x; y = v.y; z = v.z; return *this; }
	template <typename U>
	constexpr Vector3<T>& Set(const Vector3<U> & v) { x = static_cast<T>(v.x); y = static_cast<T>(v.y); z = static_cast<T>(v.z); return *this; }
	constexpr Vector3<T>& Set(const T & s) { x = s; y = s; z = s; return *this; }
	constexpr Vector3<T>& Set(const T * a) { x = a[0]; y = a[1]; z = a[2]; return *this; }
	constexpr Vector3<T>& Set(const T & s1, const T & s2, const T & s3) { x = s1; y = s2; z = s3; return *this; }
	constexpr Vector3<T>& Set(const Vector2<T> & v12, const T & s3) { x = v12.x; y = v12.y; z = s3; return *this; }
	constexpr Vector3<T>& Set(const T & s1, const Vector2<T> & v23) { x = s1; y = v23.y; z = v23.y; return *this; }

	constexpr T& operator()(U32 i) { if (i == 0) return x; else if (i == 1) return y; return z; }
	constexpr const T& operator()(U32 i) const { if (i == 0) return x; else if (i == 1) return y; return z; }
	constexpr T& operator[](U32 i) { if (i == 0) return x; else if (i == 1) return y; return z; }
	constexpr const T& operator[](U32 i) const { if (i == 0) return x; else if (i == 1) return y; return z; }

	constexpr Vector3<T>& operator=(const Vector3<T> & vec) { x = vec.x; y = vec.y; z = vec.z; return *this; }
	constexpr const Vector3<T>& operator+() const { return *this; }
	constexpr Vector3<T> operator-() const { return Vector3<T>(-x, -y, -z); }

	constexpr Vector3<T> operator+(const Vector3<T> & vec) const { return Vector3<T>(x + vec.x, y + vec.y, z + vec.z); }
	constexpr Vector3<T> operator-(const Vector3<T> & vec) const { return Vector3<T>(x - vec.x, y - vec.y, z - vec.z); }
	constexpr Vector3<T> operator*(const Vector3<T> & vec) const { return Vector3<T>(x * vec.x, y * vec.y, z * vec.z); }
	constexpr Vector3<T> operator/(const Vector3<T> & vec) const { return Vector3<T>(x / vec.x, y / vec.y, z / vec.z); }
	constexpr Vector3<T>& operator+=(const Vector3<T> & vec) { x += vec.x; y += vec.y; z += vec.z; return *this; }
	constexpr Vector3<T>& operator-=(const Vector3<T> & vec) { x -= vec.x; y -= vec.y; z -= vec.z; return *this; }
	constexpr Vector3<T>& operator*=(const Vector3<T> & vec) { x *= vec.x; y *= vec.y; z *= vec.z; return *this; }
	constexpr Vector3<T>& operator/=(const Vector3<T> & vec) { x /= vec.x; y /= vec.y; z /= vec.z; return *this; }

	constexpr Vector3<T> operator+(T scalar) const { return Vector3<T>(x + scalar, y + scalar, z + scalar); }
	constexpr Vector3<T> operator-(T scalar) const { return Vector3<T>(x - scalar, y - scalar, z - scalar); }
	constexpr Vector3<T> operator*(T scalar) const { return Vector3<T>(x * scalar, y * scalar, z * scalar); }
	constexpr Vector3<T> operator/(T scalar) const { const T inv = T(1) / scalar; return Vector3<T>(x * inv, y * inv, z * inv); }
	constexpr Vector3<T>& operator+=(T scalar) { x += scalar; y += scalar; z += scalar; return *this; }
	constexpr Vector3<T>& operator-=(T scalar) { x -= scalar; y -= scalar; z -= scalar; return *this; }
	constexpr Vector3<T>& operator*=(T scalar) { x *= scalar; y *= scalar; z *= scalar; return *this; }
	constexpr Vector3<T>& operator/=(T scalar) { const T inv = T(1) / scalar; x *= inv; y *= inv; z *= inv; return *this; }

	constexpr bool IsZero() const { return operator==(Zero()); }
	constexpr bool operator==(const Vector3<T> & vec) const { return Math::Equals(x, vec.x) && Math::Equals(y, vec.y) && Math::Equals(z, vec.z); }
	constexpr bool operator!=(const Vector3<T> & vec) const { return !operator==(vec); }
	constexpr bool operator<(const Vector3<T> & vec) const { return x < vec.x&& y < vec.y&& z < vec.z; }
	constexpr bool operator<=(const Vector3<T> & vec) const { return x <= vec.x && y <= vec.y && z <= vec.z; }
	constexpr bool operator>(const Vector3<T> & vec) const { return !operator<=(vec); }
	constexpr bool operator>=(const Vector3<T> & vec) const { return !operator<(vec); }
	static constexpr bool Equals(const Vector3<T> & v1, const Vector3<T> & v2, const T & epsilon = T(Math::Epsilon)) { return Math::Equals(v1.x, v2.x, epsilon) && Math::Equals(v1.y, v2.y, epsilon) && Math::Equals(v1.z, v2.z, epsilon); }

	constexpr Vector2<T> xy() const { return Vector2<T>(x, y); }
	constexpr Vector2<T> xz() const { return Vector2<T>(x, z); }
	constexpr Vector2<T> yz() const { return Vector2<T>(y, z); }

	constexpr T DotProduct(const Vector3<T> & v) const { return x * v.x + y * v.y + z * v.z; }
	static constexpr T DotProduct(const Vector3<T> & v1, const Vector3<T> & v2) { return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z; }

	constexpr T GetSquaredLength() const { return DotProduct(*this); }
	inline T GetLength() const { return Math::FastSqrt(GetSquaredLength()); }
	inline Vector3<T>& SetLength(const T & length, T * oldLength = nullptr)
	{
		const T currentLength = GetLength();
		if (oldLength != nullptr)
		{
			*oldLength = currentLength;
		}
		const T factor = length / currentLength;
		x *= factor;
		y *= factor;
		z *= factor;
		return *this;
	}

	constexpr bool IsNormalized() const { return Math::Equals(GetSquaredLength(), T(1)); }
	inline Vector3<T>& Normalize(T * oldLength = nullptr) { return SetLength(T(1), oldLength); }
	inline Vector3<T> Normalized(T * oldLength = nullptr) const { return Vector3<T>(*this).Normalize(oldLength); }

	constexpr Vector3<T> CrossProduct(const Vector3<T> & v) const { return Vector3<T>(y * v.z - z * v.y, z * v.x - x * v.z, x * v.y - y * v.x); }
	static constexpr Vector3<T> CrossProduct(const Vector3<T> & v1, const Vector3<T> & v2) { return Vector3<T>(v1.y * v2.z - v1.z * v2.y, v1.z * v2.x - v1.x * v2.z, v1.x * v2.y - v1.y * v2.x); }

	static constexpr Vector3<T> Lerp(const Vector3<T> & v1, const Vector3<T> & v2, const T & percent) { const T one_minus_percent = T(1) - percent; return Vector3<T>(one_minus_percent * v1.x + percent * v2.x, one_minus_percent * v1.y + percent * v2.y, one_minus_percent * v1.z + percent * v2.z); }

	constexpr Vector3<T>& Maximize(const Vector3<T> & v) { if (v.x > x) x = v.x; if (v.y > y) y = v.y; if (v.z > z) z = v.z; return *this; }
	static constexpr Vector3<T> Maximum(const Vector3<T> & v1, const Vector3<T> & v2) { return Vector3<T>(Math::Max(v1.x, v2.x), Math::Max(v1.y, v2.y), Math::Max(v1.z, v2.z)); }

	constexpr Vector3<T>& Minimize(const Vector3<T> & v) { if (v.x < x) x = v.x; if (v.y < y) y = v.y; if (v.z < z) z = v.z; return *this; }
	static constexpr Vector3<T> Minimum(const Vector3<T> & v1, const Vector3<T> & v2) { return Vector3<T>(Math::Min(v1.x, v2.x), Math::Min(v1.y, v2.y), Math::Min(v1.z, v2.z)); }

	static constexpr Vector3<T> Unit() { return Vector3<T>(1, 1, 1); }
	static constexpr Vector3<T> UnitX() { return Vector3<T>(1, 0, 0); }
	static constexpr Vector3<T> UnitY() { return Vector3<T>(0, 1, 0); }
	static constexpr Vector3<T> UnitZ() { return Vector3<T>(0, 0, 1); }
	static constexpr Vector3<T> Zero() { return Vector3<T>(0, 0, 0); }

	T x;
	T y;
	T z;
};

template <typename T> constexpr Vector3<T> operator+(const T& scalar, const Vector3<T>& vector) { return vector + scalar; }
template <typename T> constexpr Vector3<T> operator-(const T& scalar, const Vector3<T>& vector) { return scalar + (-vector); }
template <typename T> constexpr Vector3<T> operator*(const T& scalar, const Vector3<T>& vector) { return vector * scalar; }
template <typename T> constexpr Vector3<T> operator/(const T& scalar, const Vector3<T>& vector) { return Vector3<T>(scalar / vector.x, scalar / vector.y, scalar / vector.z); }

typedef Vector3<F32> Vector3f;
typedef Vector3<F64> Vector3d;
typedef Vector3<I32> Vector3i;
typedef Vector3<U32> Vector3u;

typedef Vector3f vec3; // GLSL-like

// These tests checks that the config for direction and handedness is correct
static_assert(ENLIVE_DEFAULT_FORWARD == -ENLIVE_DEFAULT_BACKWARD);
static_assert(ENLIVE_DEFAULT_UP == -ENLIVE_DEFAULT_DOWN);
static_assert(ENLIVE_DEFAULT_RIGHT == -ENLIVE_DEFAULT_LEFT);
static_assert((ENLIVE_DEFAULT_HANDEDNESS == en::Math::Handedness::Right && ENLIVE_DEFAULT_LEFT.CrossProduct(ENLIVE_DEFAULT_UP) == ENLIVE_DEFAULT_FORWARD) || (ENLIVE_DEFAULT_HANDEDNESS == en::Math::Handedness::Left && ENLIVE_DEFAULT_RIGHT.CrossProduct(ENLIVE_DEFAULT_UP) == ENLIVE_DEFAULT_FORWARD));
static_assert((ENLIVE_DEFAULT_HANDEDNESS == en::Math::Handedness::Right && ENLIVE_DEFAULT_UP.CrossProduct(ENLIVE_DEFAULT_FORWARD) == ENLIVE_DEFAULT_LEFT) || (ENLIVE_DEFAULT_HANDEDNESS == en::Math::Handedness::Left && ENLIVE_DEFAULT_UP.CrossProduct(ENLIVE_DEFAULT_FORWARD) == ENLIVE_DEFAULT_RIGHT));
static_assert((ENLIVE_DEFAULT_HANDEDNESS == en::Math::Handedness::Right && ENLIVE_DEFAULT_FORWARD.CrossProduct(ENLIVE_DEFAULT_LEFT) == ENLIVE_DEFAULT_UP) || (ENLIVE_DEFAULT_HANDEDNESS == en::Math::Handedness::Left && ENLIVE_DEFAULT_FORWARD.CrossProduct(ENLIVE_DEFAULT_RIGHT) == ENLIVE_DEFAULT_UP));

} // namespace en

#endif // ENLIVE_MODULE_MATH