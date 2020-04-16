#pragma once

#include <Enlivengine/Math/Utilities.hpp>
#include <Enlivengine/Math/Vector3.hpp>

namespace en
{

template <typename T>
class Vector4
{
public:
	static constexpr int d{ 4 };

	inline Vector4();
	inline Vector4(const Vector4<T>& v);
	template <typename U>
	explicit inline Vector4(const Vector4<U>& v);
	explicit inline Vector4(const T& s);
	explicit inline Vector4(const T* a);
	inline Vector4(const T& s1, const T& s2, const T& s3, const T& s4);
	inline Vector4(const Vector3<T>& v123, const T& s4);
	inline Vector4(const Vector2<T>& v12, const Vector2<T>& v34);
	~Vector4() = default;

	inline Vector4<T>& set(const Vector4<T> & v);
	template <typename U>
	inline Vector4<T>& set(const Vector4<U> & v);
	inline Vector4<T>& set(const T & s);
	inline Vector4<T>& set(const T * a);
	inline Vector4<T>& set(const T & s1, const T & s2, const T & s3, const T & s4);
	inline Vector4<T>& set(const Vector3<T> & v123, const T & s4);
	inline Vector4<T>& set(const Vector2<T> & v12, const Vector2<T> & v34);

	inline T& operator()(U32 i);
	inline const T& operator()(U32 i) const;
	inline T& operator[](U32 i);
	inline const T& operator[](U32 i) const;

	inline Vector4<T>& operator=(const Vector4<T> & vec);
	inline const Vector4<T>& operator+() const;
	inline Vector4<T> operator-() const;

	inline Vector4<T> operator+(const Vector4<T> & vec) const;
	inline Vector4<T> operator-(const Vector4<T> & vec) const;
	inline Vector4<T> operator*(const Vector4<T> & vec) const;
	inline Vector4<T> operator/(const Vector4<T> & vec) const;
	inline Vector4<T>& operator+=(const Vector4<T> & vec);
	inline Vector4<T>& operator-=(const Vector4<T> & vec);
	inline Vector4<T>& operator*=(const Vector4<T> & vec);
	inline Vector4<T>& operator/=(const Vector4<T> & vec);

	inline Vector4<T> operator+(T scale) const;
	inline Vector4<T> operator-(T scale) const;
	inline Vector4<T> operator*(T scale) const;
	inline Vector4<T> operator/(T scale) const;
	inline Vector4<T>& operator+=(T scale);
	inline Vector4<T>& operator-=(T scale);
	inline Vector4<T>& operator*=(T scale);
	inline Vector4<T>& operator/=(T scale);

	inline bool isZero() const;
	inline bool operator==(const Vector4<T> & vec) const;
	inline bool operator!=(const Vector4<T> & vec) const;
	inline bool operator<(const Vector4<T> & vec) const;
	inline bool operator<=(const Vector4<T> & vec) const;
	inline bool operator>(const Vector4<T> & vec) const;
	inline bool operator>=(const Vector4<T> & vec) const;
	static inline bool equalsVector(const Vector4<T> & v1, const Vector4<T> & v2, const T & epsilon = std::numeric_limits<T>::epsilon());

	inline Vector3<T> xyz() const;
	inline Vector2<T> xy() const;
	inline Vector2<T> xz() const;
	inline Vector2<T> yz() const;
	inline Vector2<T> zw() const;

	inline T dotProduct(const Vector4<T> & v) const;
	static inline T dotProduct(const Vector4<T> & v1, const Vector4<T> & v2);

	inline T getSquaredLength() const;
	inline T getLength() const;

	inline Vector4<T>& setLength(const T & length, T * oldLength = nullptr);

	inline Vector4<T>& normalize(T * oldLength = nullptr);
	inline Vector4<T> normalized(T * oldLength = nullptr) const;

	inline Vector3<T> crossProduct(const Vector4<T> & v) const;
	static inline Vector3<T> crossProduct(const Vector4<T> & v1, const Vector4<T> & v2);

	static inline Vector4<T> lerp(const Vector4<T> & v1, const Vector4<T> & v2, const T & percent);

	inline Vector4<T>& maximize(const Vector4<T> & v);
	static inline Vector4<T> maximum(const Vector4<T> & v1, const Vector4<T> & v2);

	inline Vector4<T>& minimize(const Vector4<T> & v);
	static inline Vector4<T> minimum(const Vector4<T> & v1, const Vector4<T> & v2);

	inline Vector4<T>& makeUnit();
	inline Vector4<T>& makeUnitX();
	inline Vector4<T>& makeUnitY();
	inline Vector4<T>& makeUnitZ();
	inline Vector4<T>& makeUnitW();
	inline Vector4<T>& makeZero();

	static const Vector4<T> unit;
	static const Vector4<T> unitX;
	static const Vector4<T> unitY;
	static const Vector4<T> unitZ;
	static const Vector4<T> unitW;
	static const Vector4<T> zero;

	static const Vector4<T> forward;
	static const Vector4<T> back;
	static const Vector4<T> up;
	static const Vector4<T> down;
	static const Vector4<T> right;
	static const Vector4<T> left;

	T x;
	T y;
	T z;
	T w;
};

template <typename T> Vector4<T> operator+(const T& scale, const Vector4<T>& vector);
template <typename T> Vector4<T> operator-(const T& scale, const Vector4<T>& vector);
template <typename T> Vector4<T> operator*(const T& scale, const Vector4<T>& vector);
template <typename T> Vector4<T> operator/(const T& scale, const Vector4<T>& vector);

template <typename T> T dot(const Vector4<T>& v1, const Vector4<T>& v2);

template <typename T> Vector4<T> normalize(const Vector4<T>& vector, T* oldLength = nullptr);

template<typename T>
inline Vector4<T>::Vector4()
	: x(T(0))
	, y(T(0))
	, z(T(0))
	, w(T(0))
{
}

template<typename T>
inline Vector4<T>::Vector4(const Vector4<T>& v)
	: x(v.x)
	, y(v.y)
	, z(v.z)
	, w(v.w)
{
}

template<typename T>
template<typename U>
inline Vector4<T>::Vector4(const Vector4<U>& v)
	: x(static_cast<T>(v.x))
	, y(static_cast<T>(v.y))
	, z(static_cast<T>(v.z))
	, w(static_cast<T>(v.w))
{
}

template<typename T>
inline Vector4<T>::Vector4(const T& s)
	: x(s)
	, y(s)
	, z(s)
	, w(s)
{
}

template<typename T>
inline Vector4<T>::Vector4(const T* a)
	: x(a[0])
	, y(a[1])
	, z(a[2])
	, w(a[3])
{
}

template<typename T>
inline Vector4<T>::Vector4(const T& s1, const T& s2, const T& s3, const T& s4)
	: x(s1)
	, y(s2)
	, z(s3)
	, w(s4)
{
}

template<typename T>
inline Vector4<T>::Vector4(const Vector3<T>& v123, const T& s4)
	: x(v123.x)
	, y(v123.y)
	, z(v123.z)
	, w(s4)
{
}

template<typename T>
inline Vector4<T>::Vector4(const Vector2<T>& v12, const Vector2<T>& v34)
	: x(v12.x)
	, y(v12.y)
	, z(v34.x)
	, w(v34.y)
{
}

template<typename T>
inline Vector4<T>& Vector4<T>::set(const Vector4<T>& v)
{
	x = v.x;
	y = v.y;
	z = v.z;
	w = v.w;
	return *this;
}

template<typename T>
template<typename U>
inline Vector4<T>& Vector4<T>::set(const Vector4<U>& v)
{
	x = static_cast<T>(v.x);
	y = static_cast<T>(v.y);
	z = static_cast<T>(v.z);
	w = static_cast<T>(v.w);
	return *this;
}

template<typename T>
inline Vector4<T>& Vector4<T>::set(const T& s)
{
	x = s;
	y = s;
	z = s;
	w = s;
	return *this;
}

template<typename T>
inline Vector4<T>& Vector4<T>::set(const T* a)
{
	x = a[0];
	y = a[1];
	z = a[2];
	w = a[3];
	return *this;
}

template<typename T>
inline Vector4<T>& Vector4<T>::set(const T& s1, const T& s2, const T& s3, const T& s4)
{
	x = s1;
	y = s2;
	z = s3;
	w = s4;
	return *this;
}

template<typename T>
inline Vector4<T>& Vector4<T>::set(const Vector3<T>& v123, const T& s4)
{
	x = v123.x;
	y = v123.y;
	z = v123.z;
	w = s4;
	return *this;
}

template<typename T>
inline Vector4<T>& Vector4<T>::set(const Vector2<T>& v12, const Vector2<T>& v34)
{
	x = v12.x;
	y = v12.y;
	z = v34.x;
	w = v34.y;
	return *this;
}

template<typename T>
inline T& Vector4<T>::operator()(U32 i)
{
	assert(i < 4);
	if (i == 0) return x;
	else if (i == 1) return y;
	else if (i == 2) return z;
	return w;
}

template<typename T>
inline const T& Vector4<T>::operator()(U32 i) const
{
	assert(i < 4);
	if (i == 0) return x;
	else if (i == 1) return y;
	else if (i == 2) return z;
	return w;
}

template<typename T>
inline T& Vector4<T>::operator[](U32 i)
{
	assert(i < 4);
	if (i == 0) return x;
	else if (i == 1) return y;
	else if (i == 2) return z;
	return w;
}

template<typename T>
inline const T& Vector4<T>::operator[](U32 i) const
{
	assert(i < 4);
	if (i == 0) return x;
	else if (i == 1) return y;
	else if (i == 2) return z;
	return w;
}

template<typename T>
inline Vector4<T>& Vector4<T>::operator=(const Vector4<T>& vec)
{
	x = vec.x;
	y = vec.y;
	z = vec.z;
	w = vec.w;
	return *this;
}

template<typename T>
inline const Vector4<T>& Vector4<T>::operator+() const
{
	return *this;
}

template<typename T>
inline Vector4<T> Vector4<T>::operator-() const
{
	return Vector4<T>(-x, -y, -z, -w);
}

template<typename T>
inline Vector4<T> Vector4<T>::operator+(const Vector4<T>& vec) const
{
	return Vector4<T>(x + vec.x, y + vec.y, z + vec.z, w + vec.w);
}

template<typename T>
inline Vector4<T> Vector4<T>::operator-(const Vector4<T>& vec) const
{
	return Vector4<T>(x - vec.x, y - vec.y, z - vec.z, w - vec.w);
}

template<typename T>
inline Vector4<T> Vector4<T>::operator*(const Vector4<T>& vec) const
{
	return Vector4<T>(x * vec.x, y * vec.y, z * vec.z, w * vec.w);
}

template<typename T>
inline Vector4<T> Vector4<T>::operator/(const Vector4<T>& vec) const
{
	assert(vec.x != T(0));
	assert(vec.y != T(0));
	assert(vec.z != T(0));
	assert(vec.w != T(0));
	return Vector4<T>(x / vec.x, y / vec.y, z / vec.z, w / vec.w);
}

template<typename T>
inline Vector4<T>& Vector4<T>::operator+=(const Vector4<T>& vec)
{
	x += vec.x;
	y += vec.y;
	z += vec.z;
	w += vec.w;
	return *this;
}

template<typename T>
inline Vector4<T>& Vector4<T>::operator-=(const Vector4<T>& vec)
{
	x -= vec.x;
	y -= vec.y;
	z -= vec.z;
	w -= vec.w;
	return *this;
}

template<typename T>
inline Vector4<T>& Vector4<T>::operator*=(const Vector4<T>& vec)
{
	x *= vec.x;
	y *= vec.y;
	z *= vec.z;
	w *= vec.w;
	return *this;
}

template<typename T>
inline Vector4<T>& Vector4<T>::operator/=(const Vector4<T>& vec)
{
	assert(vec.x != T(0));
	assert(vec.y != T(0));
	assert(vec.z != T(0));
	assert(vec.w != T(0));
	x /= vec.x;
	y /= vec.y;
	z /= vec.z;
	w /= vec.w;
	return *this;
}

template<typename T>
inline Vector4<T> Vector4<T>::operator+(T scale) const
{
	return Vector4<T>(x + scale, y + scale, z + scale, w + scale);
}

template<typename T>
inline Vector4<T> Vector4<T>::operator-(T scale) const
{
	return Vector4<T>(x - scale, y - scale, z - scale, w - scale);
}

template<typename T>
inline Vector4<T> Vector4<T>::operator*(T scale) const
{
	return Vector4<T>(x * scale, y * scale, z * scale, w * scale);
}

template<typename T>
inline Vector4<T> Vector4<T>::operator/(T scale) const
{
	assert(scale != T(0));
	const T inv = 1 / scale;
	return Vector4<T>(x * inv, y * inv, z * inv, w * inv);
}

template<typename T>
inline Vector4<T>& Vector4<T>::operator+=(T scale)
{
	x += scale;
	y += scale;
	z += scale;
	w += scale;
	return *this;
}

template<typename T>
inline Vector4<T>& Vector4<T>::operator-=(T scale)
{
	x -= scale;
	y -= scale;
	z -= scale;
	w -= scale;
	return *this;
}

template<typename T>
inline Vector4<T>& Vector4<T>::operator*=(T scale)
{
	x *= scale;
	y *= scale;
	z *= scale;
	w *= scale;
	return *this;
}

template<typename T>
inline Vector4<T>& Vector4<T>::operator/=(T scale)
{
	assert(scale != T(0));
	const T inv = 1 / scale;
	x *= inv;
	y *= inv;
	z *= inv;
	w *= inv;
	return *this;
}

template<typename T>
inline bool Vector4<T>::isZero() const
{
	return operator==(Vector4<T>::zero);
}

template<typename T>
inline bool Vector4<T>::operator==(const Vector4<T>& vec) const
{
	if (!Math::Equals(x, vec.x)) return false;
	if (!Math::Equals(y, vec.y)) return false;
	if (!Math::Equals(z, vec.z)) return false;
	return Math::Equals(w, vec.w);
}

template<typename T>
inline bool Vector4<T>::operator!=(const Vector4<T>& vec) const
{
	return !operator==(vec);
}

template<typename T>
inline bool Vector4<T>::operator<(const Vector4<T>& vec) const
{
	if (x >= vec.x) return false;
	if (y >= vec.y) return false;
	if (z >= vec.z) return false;
	return (w < vec.w);
}

template<typename T>
inline bool Vector4<T>::operator<=(const Vector4<T>& vec) const
{
	if (x > vec.x) return false;
	if (y > vec.y) return false;
	if (z > vec.z) return false;
	return (w <= vec.w);
}

template<typename T>
inline bool Vector4<T>::operator>(const Vector4<T>& vec) const
{
	return !operator<=(vec);
}

template<typename T>
inline bool Vector4<T>::operator>=(const Vector4<T>& vec) const
{
	return !operator<(vec);
}

template<typename T>
inline bool Vector4<T>::equalsVector(const Vector4<T>& v1, const Vector4<T>& v2, const T& epsilon)
{
	if (!Math::Equals(v1.x, v2.x, epsilon)) return false;
	if (!Math::Equals(v1.y, v2.y, epsilon)) return false;
	if (!Math::Equals(v1.z, v2.z, epsilon)) return false;
	return Math::Equals(v1.w, v2.w, epsilon);
}

template<typename T>
inline Vector3<T> Vector4<T>::xyz() const
{
	return Vector3<T>(x, y, z);
}

template<typename T>
inline Vector2<T> Vector4<T>::xy() const
{
	return Vector2<T>(x ,y);
}

template<typename T>
inline Vector2<T> Vector4<T>::xz() const
{
	return Vector2<T>(x, z);
}

template<typename T>
inline Vector2<T> Vector4<T>::yz() const
{
	return Vector2<T>(y, z);
}

template<typename T>
inline Vector2<T> Vector4<T>::zw() const
{
	return Vector2<T>(z, w);
}

template<typename T>
inline T Vector4<T>::dotProduct(const Vector4<T>& v) const
{
	return x * v.x + y * v.y + z * v.z + w * v.w;
}

template<typename T>
inline T Vector4<T>::dotProduct(const Vector4<T>& v1, const Vector4<T>& v2)
{
	return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z + v1.w * v2.w;
}

template<typename T>
inline T Vector4<T>::getSquaredLength() const
{
	return dotProduct(*this);
}

template<typename T>
inline T Vector4<T>::getLength() const
{
	return Math::Sqrt(getSquaredLength());
}

template<typename T>
inline Vector4<T>& Vector4<T>::setLength(const T& length, T* oldLength)
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
	w *= factor;
	return *this;
}

template<typename T>
inline Vector4<T>& Vector4<T>::normalize(T* oldLength)
{
	return setLength(T(1), oldLength);
}

template<typename T>
inline Vector4<T> Vector4<T>::normalized(T* oldLength) const
{
	return Vector4<T>(*this).normalize(oldLength);
}

template<typename T>
inline Vector3<T> Vector4<T>::crossProduct(const Vector4<T>& v) const
{
	return Vector3<T>(y * v.z - z * v.y, z * v.x - x * v.z, x * v.y - y * v.x);
}

template<typename T>
inline Vector3<T> Vector4<T>::crossProduct(const Vector4<T>& v1, const Vector4<T>& v2)
{
	return Vector3<T>(v1.y * v2.z - v1.z * v2.y, v1.z * v2.x - v1.x * v2.z, v1.x * v2.y - v1.y * v2.x);
}

template<typename T>
inline Vector4<T> Vector4<T>::lerp(const Vector4<T>& v1, const Vector4<T>& v2, const T& percent)
{
	const T one_minus_percent = T(1) - percent;
	return Vector4<T>(one_minus_percent * v1.x + percent * v2.x, one_minus_percent * v1.y + percent * v2.y, one_minus_percent * v1.z + percent * v2.z, one_minus_percent * v1.w + percent * v2.w);
}

template<typename T>
inline Vector4<T>& Vector4<T>::maximize(const Vector4<T>& v)
{
	if (v.x > x) x = v.x;
	if (v.y > y) y = v.y;
	if (v.z > z) z = v.z;
	if (v.w > w) w = v.w;
	return *this;
}

template<typename T>
inline Vector4<T> Vector4<T>::maximum(const Vector4<T>& v1, const Vector4<T>& v2)
{
	return Vector4<T>(Math::Max(v1.x, v2.x), Math::Max(v1.y, v2.y), Math::Max(v1.z, v2.z), Math::Max(v1.w, v2.w));
}

template<typename T>
inline Vector4<T>& Vector4<T>::minimize(const Vector4<T>& v)
{
	if (v.x < x) x = v.x;
	if (v.y < y) y = v.y;
	if (v.z < z) z = v.z;
	if (v.w < w) w = v.w;
	return *this;
}

template<typename T>
inline Vector4<T> Vector4<T>::minimum(const Vector4<T>& v1, const Vector4<T>& v2)
{
	return Vector4<T>(Math::Min(v1.x, v2.x), Math::Min(v1.y, v2.y), Math::Min(v1.z, v2.z), Math::Min(v1.w, v2.w));
}

template<typename T>
inline Vector4<T>& Vector4<T>::makeUnit()
{
	return set(T(1), T(1), T(1), T(1));
}

template<typename T>
inline Vector4<T>& Vector4<T>::makeUnitX()
{
	return set(T(1), T(0), T(0), T(0));
}

template<typename T>
inline Vector4<T>& Vector4<T>::makeUnitY()
{
	return set(T(0), T(1), T(0), T(0));
}

template<typename T>
inline Vector4<T>& Vector4<T>::makeUnitZ()
{
	return set(T(0), T(0), T(1), T(0));
}

template<typename T>
inline Vector4<T>& Vector4<T>::makeUnitW()
{
	return set(T(0), T(0), T(0), T(1));
}

template<typename T>
inline Vector4<T>& Vector4<T>::makeZero()
{
	return set(T(0), T(0), T(0), T(0));
}

template<typename T>
Vector4<T> operator+(const T& scale, const Vector4<T>& vector)
{
	return vector + scale;
}

template<typename T>
Vector4<T> operator-(const T& scale, const Vector4<T>& vector)
{
	return scale + (vector.operator-());
}

template<typename T>
inline Vector4<T> operator*(const T& scale, const Vector4<T>& vector)
{
	return vector * scale;
}

template<typename T>
Vector4<T> operator/(const T & scale, const Vector4<T>& vector)
{
	assert(vector[0] != T(0));
	assert(vector[1] != T(0));
	assert(vector[2] != T(0));
	assert(vector[3] != T(0));
	return Vector4<T>(scale / vector[0], scale / vector[1], scale / vector[2], scale / vector[3]);
}

template<typename T>
T dot(const Vector4<T>& v1, const Vector4<T>& v2)
{
	return v1.dotProduct(v2);
}

template<typename T>
Vector4<T> normalize(const Vector4<T>& vector, T* oldLength)
{
	return vector.normalized(oldLength);
}

template <typename T>
bool equalsVector(const Vector4<T>& v1, const Vector4<T>& v2, const T& epsilon)
{
	if (!Math::Equals(v1.x, v2.x, epsilon)) return false;
	if (!Math::Equals(v1.y, v2.y, epsilon)) return false;
	if (!Math::Equals(v1.z, v2.z, epsilon)) return false;
	return Math::Equals(v1.w, v2.w, epsilon);
}

template <typename T> const Vector4<T> Vector4<T>::unit(1, 1, 1, 1);
template <typename T> const Vector4<T> Vector4<T>::unitX(1, 0, 0, 0);
template <typename T> const Vector4<T> Vector4<T>::unitY(0, 1, 0, 0);
template <typename T> const Vector4<T> Vector4<T>::unitZ(0, 0, 1, 0);
template <typename T> const Vector4<T> Vector4<T>::unitW(0, 0, 0, 1);
template <typename T> const Vector4<T> Vector4<T>::zero(0, 0, 0, 0);

template <typename T> const Vector4<T> Vector4<T>::forward(ENLIVE_MATH_FORWARD, 0);
template <typename T> const Vector4<T> Vector4<T>::back(ENLIVE_MATH_BACK, 0);
template <typename T> const Vector4<T> Vector4<T>::up(ENLIVE_MATH_UP, 0);
template <typename T> const Vector4<T> Vector4<T>::down(ENLIVE_MATH_DOWN, 0);
template <typename T> const Vector4<T> Vector4<T>::right(ENLIVE_MATH_RIGHT, 0);
template <typename T> const Vector4<T> Vector4<T>::left(ENLIVE_MATH_LEFT, 0);

typedef Vector4<float>			Vector4f;
typedef Vector4<double>			Vector4d;
typedef Vector4<int>			Vector4i;
typedef Vector4<unsigned int>	Vector4u;

typedef Vector4f vec4; // GLSL-like

} // namespace en