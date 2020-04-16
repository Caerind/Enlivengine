#pragma once

#include <Enlivengine/Math/Utilities.hpp>
#include <Enlivengine/Math/Vector2.hpp>

// TODO : Vector3 Aligned ?
// TODO : constexpr

namespace en
{

template <typename T>
class Vector3
{
public:
	static constexpr I32 d = 3;

	inline Vector3();
	inline Vector3(const Vector3<T>& v);
	template <typename U>
	explicit inline Vector3(const Vector3<U>& v);
	explicit inline Vector3(const T& s);
	explicit inline Vector3(const T* a);
	inline Vector3(const T& s1, const T& s2, const T& s3);
	inline Vector3(const Vector2<T>& v12, const T& s3);
	~Vector3() = default;

	inline Vector3<T>& set(const Vector3<T> & v);
	template <typename U>
	inline Vector3<T>& set(const Vector3<U> & v);
	inline Vector3<T>& set(const T & s);
	inline Vector3<T>& set(const T * a);
	inline Vector3<T>& set(const T & s1, const T & s2, const T & s3);
	inline Vector3<T>& set(const Vector2<T> & v12, const T & s3);

	inline T& operator()(U32 i);
	inline const T& operator()(U32 i) const;
	inline T& operator[](U32 i);
	inline const T& operator[](U32 i) const;

	inline Vector3<T>& operator=(const Vector3<T> & vec);
	inline const Vector3<T>& operator+() const;
	inline Vector3<T> operator-() const;

	inline Vector3<T> operator+(const Vector3<T> & vec) const;
	inline Vector3<T> operator-(const Vector3<T> & vec) const;
	inline Vector3<T> operator*(const Vector3<T> & vec) const;
	inline Vector3<T> operator/(const Vector3<T> & vec) const;
	inline Vector3<T>& operator+=(const Vector3<T> & vec);
	inline Vector3<T>& operator-=(const Vector3<T> & vec);
	inline Vector3<T>& operator*=(const Vector3<T> & vec);
	inline Vector3<T>& operator/=(const Vector3<T> & vec);

	inline Vector3<T> operator+(T scale) const;
	inline Vector3<T> operator-(T scale) const;
	inline Vector3<T> operator*(T scale) const;
	inline Vector3<T> operator/(T scale) const;
	inline Vector3<T>& operator+=(T scale);
	inline Vector3<T>& operator-=(T scale);
	inline Vector3<T>& operator*=(T scale);
	inline Vector3<T>& operator/=(T scale);

	inline bool isZero() const;
	inline bool operator==(const Vector3<T> & vec) const;
	inline bool operator!=(const Vector3<T> & vec) const;
	inline bool operator<(const Vector3<T> & vec) const;
	inline bool operator<=(const Vector3<T> & vec) const;
	inline bool operator>(const Vector3<T> & vec) const;
	inline bool operator>=(const Vector3<T> & vec) const;
	static inline bool Equals(const Vector3<T> & v1, const Vector3<T> & v2, const T & epsilon = std::numeric_limits<T>::epsilon());

	inline Vector2<T> xy() const;
	inline Vector2<T> xz() const;
	inline Vector2<T> yz() const;

	inline T dotProduct(const Vector3<T> & v) const;
	static inline T dotProduct(const Vector3<T> & v1, const Vector3<T> & v2);

	inline T getSquaredLength() const;
	inline T getLength() const;

	inline Vector3<T>& setLength(const T & length, T * oldLength = nullptr);

	inline Vector3<T>& normalize(T * oldLength = nullptr);
	inline Vector3<T> normalized(T * oldLength = nullptr) const;

	inline Vector3<T> crossProduct(const Vector3<T> & v) const;
	static inline Vector3<T> crossProduct(const Vector3<T> & v1, const Vector3<T> & v2);

	inline Vector3<T> perpendicularVector() const;

	static inline Vector3<T> lerp(const Vector3<T> & v1, const Vector3<T> & v2, const T & percent);

	inline Vector3<T>& maximize(const Vector3<T> & v);
	static inline Vector3<T> maximum(const Vector3<T> & v1, const Vector3<T> & v2);

	inline Vector3<T>& minimize(const Vector3<T> & v);
	static inline Vector3<T> minimum(const Vector3<T> & v1, const Vector3<T> & v2);

	inline Vector3<T>& makeUnit();
	inline Vector3<T>& makeUnitX();
	inline Vector3<T>& makeUnitY();
	inline Vector3<T>& makeUnitZ();
	inline Vector3<T>& makeZero();

	static const Vector3<T> unit;
	static const Vector3<T> unitX;
	static const Vector3<T> unitY;
	static const Vector3<T> unitZ;
	static const Vector3<T> zero;

	static const Vector3<T> forward;
	static const Vector3<T> back;
	static const Vector3<T> up;
	static const Vector3<T> down;
	static const Vector3<T> right;
	static const Vector3<T> left;

	T x;
	T y;
	T z;
};

template <typename T> Vector3<T> operator+(const T& scale, const Vector3<T>& vector);
template <typename T> Vector3<T> operator-(const T& scale, const Vector3<T>& vector);
template <typename T> Vector3<T> operator*(const T& scale, const Vector3<T>& vector);
template <typename T> Vector3<T> operator/(const T& scale, const Vector3<T>& vector);

template <typename T> T dot(const Vector3<T>& v1, const Vector3<T>& v2);

template <typename T> Vector3<T> normalize(const Vector3<T>& vec, T* oldLength = nullptr);

template <typename T> Vector3<T> cross(const Vector3<T>& v1, const Vector3<T>& v2);

template<typename T>
inline Vector3<T>::Vector3()
	: x(T(0))
	, y(T(0))
	, z(T(0))
{
}

template<typename T>
inline Vector3<T>::Vector3(const Vector3<T>& v)
	: x(v.x)
	, y(v.y)
	, z(v.z)
{
}

template<typename T>
template<typename U>
inline Vector3<T>::Vector3(const Vector3<U>& v)
	: x(static_cast<T>(v.x))
	, y(static_cast<T>(v.y))
	, z(static_cast<T>(v.z))
{
}

template<typename T>
inline Vector3<T>::Vector3(const T& s)
	: x(s)
	, y(s)
	, z(s)
{
}

template<typename T>
inline Vector3<T>::Vector3(const T* a)
	: x(a[0])
	, y(a[1])
	, z(a[2])
{
}

template<typename T>
inline Vector3<T>::Vector3(const T& s1, const T& s2, const T& s3)
	: x(s1)
	, y(s2)
	, z(s3)
{
}

template<typename T>
inline Vector3<T>::Vector3(const Vector2<T>& v12, const T& s3)
{
	x = v12.x;
	y = v12.y;
	z = s3;
}

template<typename T>
inline Vector3<T>& Vector3<T>::set(const Vector3<T>& v)
{
	x = v.x;
	y = v.y;
	z = v.z;
	return *this;
}

template<typename T>
template<typename U>
inline Vector3<T>& Vector3<T>::set(const Vector3<U>& v)
{
	x = static_cast<T>(v.x);
	y = static_cast<T>(v.y);
	z = static_cast<T>(v.z);
	return *this;
}

template<typename T>
inline Vector3<T>& Vector3<T>::set(const T& s)
{
	x = s;
	y = s;
	z = s;
	return *this;
}

template<typename T>
inline Vector3<T>& Vector3<T>::set(const T* a)
{
	x = a[0];
	y = a[1];
	z = a[2];
	return *this;
}

template<typename T>
inline Vector3<T>& Vector3<T>::set(const T& s1, const T& s2, const T& s3)
{
	x = s1;
	y = s2;
	z = s3;
	return *this;
}

template<typename T>
inline Vector3<T>& Vector3<T>::set(const Vector2<T>& v12, const T& s3)
{
	x = v12.x;
	y = v12.y;
	z = s3;
	return *this;
}

template<typename T>
inline T& Vector3<T>::operator()(U32 i)
{
	assert(i < 3);
	if (i == 0) return x;
	else if (i == 1) return y;
	return z;
}

template<typename T>
inline const T& Vector3<T>::operator()(U32 i) const
{
	assert(i < 3);
	if (i == 0) return x;
	else if (i == 1) return y;
	return z;
}

template<typename T>
inline T& Vector3<T>::operator[](U32 i)
{
	assert(i < 3);
	if (i == 0) return x;
	else if (i == 1) return y;
	return z;
}

template<typename T>
inline const T& Vector3<T>::operator[](U32 i) const
{
	assert(i < 3);
	if (i == 0) return x;
	else if (i == 1) return y;
	return z;
}

template<typename T>
inline Vector3<T>& Vector3<T>::operator=(const Vector3<T>& vec)
{
	x = vec.x;
	y = vec.y;
	z = vec.z;
	return *this;
}

template<typename T>
inline const Vector3<T>& Vector3<T>::operator+() const
{
	return *this;
}

template<typename T>
inline Vector3<T> Vector3<T>::operator-() const
{
	return Vector3<T>(-x, -y, -z);
}

template<typename T>
inline Vector3<T> Vector3<T>::operator+(const Vector3<T>& vec) const
{
	return Vector3<T>(x + vec.x, y + vec.y, z + vec.z);
}

template<typename T>
inline Vector3<T> Vector3<T>::operator-(const Vector3<T>& vec) const
{
	return Vector3<T>(x - vec.x, y - vec.y, z - vec.z);
}

template<typename T>
inline Vector3<T> Vector3<T>::operator*(const Vector3<T>& vec) const
{
	return Vector3<T>(x * vec.x, y * vec.y, z * vec.z);
}

template<typename T>
inline Vector3<T> Vector3<T>::operator/(const Vector3<T>& vec) const
{
	assert(vec.x != T(0));
	assert(vec.y != T(0));
	assert(vec.z != T(0));
	return Vector3<T>(x / vec.x, y / vec.y, z / vec.z);
}

template<typename T>
inline Vector3<T>& Vector3<T>::operator+=(const Vector3<T>& vec)
{
	x += vec.x;
	y += vec.y;
	z += vec.z;
	return *this;
}

template<typename T>
inline Vector3<T>& Vector3<T>::operator-=(const Vector3<T>& vec)
{
	x -= vec.x;
	y -= vec.y;
	z -= vec.z;
	return *this;
}

template<typename T>
inline Vector3<T>& Vector3<T>::operator*=(const Vector3<T>& vec)
{
	x *= vec.x;
	y *= vec.y;
	z *= vec.z;
	return *this;
}

template<typename T>
inline Vector3<T>& Vector3<T>::operator/=(const Vector3<T>& vec)
{
	assert(vec.x != T(0));
	assert(vec.y != T(0));
	assert(vec.z != T(0));
	x /= vec.x;
	y /= vec.y;
	z /= vec.z;
	return *this;
}

template<typename T>
inline Vector3<T> Vector3<T>::operator+(T scale) const
{
	return Vector3<T>(x + scale, y + scale, z + scale);
}

template<typename T>
inline Vector3<T> Vector3<T>::operator-(T scale) const
{
	return Vector3<T>(x - scale, y - scale, z - scale);
}

template<typename T>
inline Vector3<T> Vector3<T>::operator*(T scale) const
{
	return Vector3<T>(x * scale, y * scale, z * scale);
}

template<typename T>
inline Vector3<T> Vector3<T>::operator/(T scale) const
{
	assert(scale != T(0));
	const T inv = 1 / scale;
	return Vector3<T>(x * inv, y * inv, z * inv);
}

template<typename T>
inline Vector3<T>& Vector3<T>::operator+=(T scale)
{
	x += scale;
	y += scale;
	z += scale;
	return *this;
}

template<typename T>
inline Vector3<T>& Vector3<T>::operator-=(T scale)
{
	x -= scale;
	y -= scale;
	z -= scale;
	return *this;
}

template<typename T>
inline Vector3<T>& Vector3<T>::operator*=(T scale)
{
	x *= scale;
	y *= scale;
	z *= scale;
	return *this;
}

template<typename T>
inline Vector3<T>& Vector3<T>::operator/=(T scale)
{
	assert(scale != T(0));
	const T inv = 1 / scale;
	x *= inv;
	y *= inv;
	z *= inv;
	return *this;
}

template<typename T>
inline bool Vector3<T>::isZero() const
{
	return operator==(Vector3<T>::zero);
}

template<typename T>
inline bool Vector3<T>::operator==(const Vector3<T>& vec) const
{
	if (!Math::Equals(x, vec.x)) return false;
	if (!Math::Equals(y, vec.y)) return false;
	return Math::Equals(z, vec.z);
}

template<typename T>
inline bool Vector3<T>::operator!=(const Vector3<T>& vec) const
{
	return !operator==(vec);
}

template<typename T>
inline bool Vector3<T>::operator<(const Vector3<T>& vec) const
{
	if (x >= vec.x) return false;
	if (y >= vec.y) return false;
	return (z < vec.z);
}

template<typename T>
inline bool Vector3<T>::operator<=(const Vector3<T>& vec) const
{
	if (x > vec.x) return false;
	if (y > vec.y) return false;
	return (z <= vec.z);
}

template<typename T>
inline bool Vector3<T>::operator>(const Vector3<T>& vec) const
{
	return !operator<=(vec);
}

template<typename T>
inline bool Vector3<T>::operator>=(const Vector3<T>& vec) const
{
	return !operator<(vec);
}

template<typename T>
inline bool Vector3<T>::Equals(const Vector3<T>& v1, const Vector3<T>& v2, const T& epsilon)
{
	if (!Math::Equals(v1.x, v2.x, epsilon)) return false;
	if (!Math::Equals(v1.y, v2.y, epsilon)) return false;
	return Math::Equals(v1.z, v2.z, epsilon);
}

template<typename T>
inline Vector2<T> Vector3<T>::xy() const
{
	return Vector2<T>(x ,y);
}

template<typename T>
inline Vector2<T> Vector3<T>::xz() const
{
	return Vector2<T>(x, z);
}

template<typename T>
inline Vector2<T> Vector3<T>::yz() const
{
	return Vector2<T>(y, z);
}

template<typename T>
inline T Vector3<T>::dotProduct(const Vector3<T>& v) const
{
	return x * v.x + y * v.y + z * v.z;
}

template<typename T>
inline T Vector3<T>::dotProduct(const Vector3<T>& v1, const Vector3<T>& v2)
{
	return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
}

template<typename T>
inline T Vector3<T>::getSquaredLength() const
{
	return dotProduct(*this);
}

template<typename T>
inline T Vector3<T>::getLength() const
{
	return Math::Sqrt(getSquaredLength());
}

template<typename T>
inline Vector3<T>& Vector3<T>::setLength(const T& length, T* oldLength)
{
	const T currentLength = getLength();
	assert(currentLength != T(0));
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

template<typename T>
inline Vector3<T>& Vector3<T>::normalize(T* oldLength)
{
	return setLength(T(1), oldLength);
}

template<typename T>
inline Vector3<T> Vector3<T>::normalized(T* oldLength) const
{
	return Vector3<T>(*this).normalize(oldLength);
}

template<typename T>
inline Vector3<T> Vector3<T>::crossProduct(const Vector3<T>& v) const
{
	return Vector3<T>(y * v.z - z * v.y, z * v.x - x * v.z, x * v.y - y * v.x);
}

template<typename T>
inline Vector3<T> Vector3<T>::crossProduct(const Vector3<T>& v1, const Vector3<T>& v2)
{
	return Vector3<T>(v1.y * v2.z - v1.z * v2.y, v1.z * v2.x - v1.x * v2.z, v1.x * v2.y - v1.y * v2.x);
}

template<typename T>
inline Vector3<T> Vector3<T>::perpendicularVector() const
{
	Vector3<T> axis = Vector3<T>::crossProduct(unitX(), *this);
	if (axis.getSquaredLength() < T(0.05)) 
	{
		axis = Vector3<T>::crossProduct(unitY(), *this);
	}
	return axis.normalize();
}

template<typename T>
inline Vector3<T> Vector3<T>::lerp(const Vector3<T>& v1, const Vector3<T>& v2, const T& percent)
{
	const T one_minus_percent = T(1) - percent;
	return Vector3<T>(one_minus_percent * v1.x + percent * v2.x, one_minus_percent * v1.y + percent * v2.y, one_minus_percent * v1.z + percent * v2.z);
}

template<typename T>
inline Vector3<T>& Vector3<T>::maximize(const Vector3<T>& v)
{
	if (v.x > x) x = v.x;
	if (v.y > y) y = v.y;
	if (v.z > z) z = v.z;
	return *this;
}

template<typename T>
inline Vector3<T> Vector3<T>::maximum(const Vector3<T>& v1, const Vector3<T>& v2)
{
	return Vector3<T>(Math::Max(v1.x, v2.x), Math::Max(v1.y, v2.y), Math::Max(v1.z, v2.z));
}

template<typename T>
inline Vector3<T>& Vector3<T>::minimize(const Vector3<T>& v)
{
	if (v.x < x) x = v.x;
	if (v.y < y) y = v.y;
	if (v.z < z) z = v.z;
	return *this;
}

template<typename T>
inline Vector3<T> Vector3<T>::minimum(const Vector3<T>& v1, const Vector3<T>& v2)
{
	return Vector3<T>(Math::Min(v1.x, v2.x), Math::Min(v1.y, v2.y), Math::Min(v1.z, v2.z));
}

template<typename T>
inline Vector3<T>& Vector3<T>::makeUnit()
{
	return set(T(1), T(1), T(1));
}

template<typename T>
inline Vector3<T>& Vector3<T>::makeUnitX()
{
	return set(T(1), T(0), T(0));
}

template<typename T>
inline Vector3<T>& Vector3<T>::makeUnitY()
{
	return set(T(0), T(1), T(0));
}

template<typename T>
inline Vector3<T>& Vector3<T>::makeUnitZ()
{
	return set(T(0), T(0), T(1));
}

template<typename T>
inline Vector3<T>& Vector3<T>::makeZero()
{
	return set(T(0), T(0), T(0));
}

template<typename T>
Vector3<T> operator+(const T& scale, const Vector3<T>& vector)
{
	return vector + scale;
}

template<typename T>
Vector3<T> operator-(const T& scale, const Vector3<T>& vector)
{
	return scale + (vector.operator-());
}

template<typename T>
inline Vector3<T> operator*(const T& scale, const Vector3<T>& vector)
{
	return vector * scale;
}

template<typename T>
Vector3<T> operator/(const T& scale, const Vector3<T>& vector)
{
	assert(vector[0] != T(0));
	assert(vector[1] != T(0));
	assert(vector[2] != T(0));
	return Vector3<T>(scale / vector[0], scale / vector[1], scale / vector[2]);
}

template<typename T>
T dot(const Vector3<T>& v1, const Vector3<T>& v2)
{
	return v1.dotProduct(v2);
}

template<typename T>
Vector3<T> normalize(const Vector3<T>& vec, T* oldLength)
{
	return vec.normalized(oldLength);
}

template<typename T>
Vector3<T> cross(const Vector3<T>& v1, const Vector3<T>& v2)
{
	return v1.crossProduct(v2);
}

template <typename T> const Vector3<T> Vector3<T>::unit(1, 1, 1);
template <typename T> const Vector3<T> Vector3<T>::unitX(1, 0, 0);
template <typename T> const Vector3<T> Vector3<T>::unitY(0, 1, 0);
template <typename T> const Vector3<T> Vector3<T>::unitZ(0, 0, 1);
template <typename T> const Vector3<T> Vector3<T>::zero(0, 0, 0);

template <typename T> const Vector3<T> Vector3<T>::forward(ENLIVE_MATH_FORWARD);
template <typename T> const Vector3<T> Vector3<T>::back(ENLIVE_MATH_BACK);
template <typename T> const Vector3<T> Vector3<T>::up(ENLIVE_MATH_UP);
template <typename T> const Vector3<T> Vector3<T>::down(ENLIVE_MATH_DOWN);
template <typename T> const Vector3<T> Vector3<T>::right(ENLIVE_MATH_RIGHT);
template <typename T> const Vector3<T> Vector3<T>::left(ENLIVE_MATH_LEFT);

typedef Vector3<Real> Vector3r;
typedef Vector3<F32> Vector3f;
typedef Vector3<I32> Vector3i;
typedef Vector3<U32> Vector3u;

typedef Vector3f vec3; // GLSL-like

} // namespace en