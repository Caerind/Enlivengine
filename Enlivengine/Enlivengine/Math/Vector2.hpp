#pragma once

#include <Enlivengine/Math/Utilities.hpp>

// TODO : constexpr

namespace en
{

template <typename T>
class Vector2
{
public:
	static constexpr I32 d = 2;

	inline Vector2();
	inline Vector2(const Vector2<T>& v);
	template <typename U>
	explicit inline Vector2(const Vector2<U>& v);
	explicit inline Vector2(const T& s);
	explicit inline Vector2(const T* a);
	inline Vector2(const T& s1, const T& s2);
	~Vector2() = default;

	inline Vector2<T>& set(const Vector2<T> & v);
	template <typename U>
	inline Vector2<T>& set(const Vector2<U> & v);
	inline Vector2<T>& set(const T & s);
	inline Vector2<T>& set(const T * a);
	inline Vector2<T>& set(const T & s1, const T & s2);

	inline T& operator()(U32 i);
	inline const T& operator()(U32 i) const;
	inline T& operator[](U32 i);
	inline const T& operator[](U32 i) const;

	inline Vector2<T>& operator=(const Vector2<T> & vec);
	inline const Vector2<T>& operator+() const;
	inline Vector2<T> operator-() const;

	inline Vector2<T> operator+(const Vector2<T> & vec) const;
	inline Vector2<T> operator-(const Vector2<T> & vec) const;
	inline Vector2<T> operator*(const Vector2<T> & vec) const;
	inline Vector2<T> operator/(const Vector2<T> & vec) const;
	inline Vector2<T>& operator+=(const Vector2<T> & vec);
	inline Vector2<T>& operator-=(const Vector2<T> & vec);
	inline Vector2<T>& operator*=(const Vector2<T> & vec);
	inline Vector2<T>& operator/=(const Vector2<T> & vec);

	inline Vector2<T> operator+(T scale) const;
	inline Vector2<T> operator-(T scale) const;
	inline Vector2<T> operator*(T scale) const;
	inline Vector2<T> operator/(T scale) const;
	inline Vector2<T>& operator+=(T scale);
	inline Vector2<T>& operator-=(T scale);
	inline Vector2<T>& operator*=(T scale);
	inline Vector2<T>& operator/=(T scale);

	inline bool isZero() const;
	inline bool operator==(const Vector2<T> & vec) const;
	inline bool operator!=(const Vector2<T> & vec) const;
	inline bool operator<(const Vector2<T> & vec) const;
	inline bool operator<=(const Vector2<T> & vec) const;
	inline bool operator>(const Vector2<T> & vec) const;
	inline bool operator>=(const Vector2<T> & vec) const;
	static inline bool Equals(const Vector2<T> & v1, const Vector2<T> & v2, const T & epsilon = std::numeric_limits<T>::epsilon());

	inline T dotProduct(const Vector2<T> & v) const;
	static inline T dotProduct(const Vector2<T> & v1, const Vector2<T> & v2);

	inline T getSquaredLength() const;
	inline T getLength() const;

	inline Vector2<T>& setLength(const T & length, T * oldLength = nullptr);

	inline Vector2<T>& normalize(T * oldLength = nullptr);
	inline Vector2<T> normalized(T * oldLength = nullptr) const;

	inline T getPolarAngle() const;
	inline Vector2<T>& setPolarAngle(T angle);

	inline Vector2<T>& rotate(T angle);
	inline Vector2<T> rotated(T angle) const;

	static inline Vector2<T> lerp(const Vector2<T> & v1, const Vector2<T> & v2, const T & percent);

	inline Vector2<T>& maximize(const Vector2<T> & v);
	static inline Vector2<T> maximum(const Vector2<T> & v1, const Vector2<T> & v2);

	inline Vector2<T>& minimize(const Vector2<T> & v);
	static inline Vector2<T> minimum(const Vector2<T> & v1, const Vector2<T> & v2);

	inline Vector2<T>& makeUnit();
	inline Vector2<T>& makeUnitX();
	inline Vector2<T>& makeUnitY();
	inline Vector2<T>& makeZero();
	inline Vector2<T>& makePolar(T angle, T length = 1);

	static inline Vector2<T> polar(T angle, T length = 1);

	static const Vector2<T> unit;
	static const Vector2<T> unitX;
	static const Vector2<T> unitY;
	static const Vector2<T> zero;

	T x;
	T y;
};

template <typename T> Vector2<T> operator+(const T& scale, const Vector2<T>& vector);
template <typename T> Vector2<T> operator-(const T& scale, const Vector2<T>& vector);
template <typename T> Vector2<T> operator*(const T& scale, const Vector2<T>& vector);
template <typename T> Vector2<T> operator/(const T& scale, const Vector2<T>& vector);

template <typename T> T dot(const Vector2<T>& v1, const Vector2<T>& v2);

template <typename T> Vector2<T> normalize(const Vector2<T>& vector, T* oldLength = nullptr);

template<typename T>
inline Vector2<T>::Vector2()
	: x(T(0))
	, y(T(0))
{
}

template<typename T>
inline Vector2<T>::Vector2(const Vector2<T>& v)
	: x(v.x)
	, y(v.y)
{
}

template<typename T>
template<typename U>
inline Vector2<T>::Vector2(const Vector2<U>& v)
	: x(static_cast<T>(v.x))
	, y(static_cast<T>(v.y))
{
}

template<typename T>
inline Vector2<T>::Vector2(const T& s)
	: x(s)
	, y(s)
{
}

template<typename T>
inline Vector2<T>::Vector2(const T* a)
	: x(a[0])
	, y(a[1])
{
}

template<typename T>
inline Vector2<T>::Vector2(const T& s1, const T& s2)
	: x(s1)
	, y(s2)
{
}

template<typename T>
inline Vector2<T>& Vector2<T>::set(const Vector2<T>& v)
{
	x = v.x;
	y = v.y;
	return *this;
}

template<typename T>
template<typename U>
inline Vector2<T>& Vector2<T>::set(const Vector2<U>& v)
{
	x = static_cast<T>(v.x);
	y = static_cast<T>(v.y);
	return *this;
}

template<typename T>
inline Vector2<T>& Vector2<T>::set(const T& s)
{
	x = s;
	y = s;
	return *this;
}

template<typename T>
inline Vector2<T>& Vector2<T>::set(const T* a)
{
	x = a[0];
	y = a[1];
	return *this;
}

template<typename T>
inline Vector2<T>& Vector2<T>::set(const T& s1, const T& s2)
{
	x = s1;
	y = s2;
	return *this;
}

template<typename T>
inline T& Vector2<T>::operator()(U32 i)
{
	assert(i < 2);
	if (i == 0) return x;
	return y;
}

template<typename T>
inline const T& Vector2<T>::operator()(U32 i) const
{
	assert(i < 2);
	if (i == 0) return x;
	return y;
}

template<typename T>
inline T& Vector2<T>::operator[](U32 i)
{
	assert(i < 2);
	if (i == 0) return x;
	return y;
}

template<typename T>
inline const T& Vector2<T>::operator[](U32 i) const
{
	assert(i < 2);
	if (i == 0) return x;
	return y;
}

template<typename T>
inline Vector2<T>& Vector2<T>::operator=(const Vector2<T>& vec)
{
	x = vec.x;
	y = vec.y;
	return *this;
}

template<typename T>
inline const Vector2<T>& Vector2<T>::operator+() const
{
	return *this;
}

template<typename T>
inline Vector2<T> Vector2<T>::operator-() const
{
	return Vector2<T>(-x, -y);
}

template<typename T>
inline Vector2<T> Vector2<T>::operator+(const Vector2<T>& vec) const
{
	return Vector2<T>(x + vec.x, y + vec.y);
}

template<typename T>
inline Vector2<T> Vector2<T>::operator-(const Vector2<T>& vec) const
{
	return Vector2<T>(x - vec.x, y - vec.y);
}

template<typename T>
inline Vector2<T> Vector2<T>::operator*(const Vector2<T>& vec) const
{
	return Vector2<T>(x * vec.x, y * vec.y);
}

template<typename T>
inline Vector2<T> Vector2<T>::operator/(const Vector2<T>& vec) const
{
	assert(vec.x != T(0));
	assert(vec.y != T(0));
	return Vector2<T>(x / vec.x, y / vec.y);
}

template<typename T>
inline Vector2<T>& Vector2<T>::operator+=(const Vector2<T>& vec)
{
	x += vec.x;
	y += vec.y;
	return *this;
}

template<typename T>
inline Vector2<T>& Vector2<T>::operator-=(const Vector2<T>& vec)
{
	x -= vec.x;
	y -= vec.y;
	return *this;
}

template<typename T>
inline Vector2<T>& Vector2<T>::operator*=(const Vector2<T>& vec)
{
	x *= vec.x;
	y *= vec.y;
	return *this;
}

template<typename T>
inline Vector2<T>& Vector2<T>::operator/=(const Vector2<T>& vec)
{
	assert(vec.x != T(0));
	assert(vec.y != T(0));
	x /= vec.x;
	y /= vec.y;
	return *this;
}

template<typename T>
inline Vector2<T> Vector2<T>::operator+(T scale) const
{
	return Vector2<T>(x + scale, y + scale);
}

template<typename T>
inline Vector2<T> Vector2<T>::operator-(T scale) const
{
	return Vector2<T>(x - scale, y - scale);
}

template<typename T>
inline Vector2<T> Vector2<T>::operator*(T scale) const
{
	return Vector2<T>(x * scale, y * scale);
}

template<typename T>
inline Vector2<T> Vector2<T>::operator/(T scale) const
{
	assert(scale != T(0));
	const T inv = 1 / scale;
	return Vector2<T>(x * inv, y * inv);
}

template<typename T>
inline Vector2<T>& Vector2<T>::operator+=(T scale)
{
	x += scale;
	y += scale;
	return *this;
}

template<typename T>
inline Vector2<T>& Vector2<T>::operator-=(T scale)
{
	x -= scale;
	y -= scale;
	return *this;
}

template<typename T>
inline Vector2<T>& Vector2<T>::operator*=(T scale)
{
	x *= scale;
	y *= scale;
	return *this;
}

template<typename T>
inline Vector2<T>& Vector2<T>::operator/=(T scale)
{
	assert(scale != T(0));
	const T inv = 1 / scale;
	x *= inv;
	y *= inv;
	return *this;
}

template<typename T>
inline bool Vector2<T>::isZero() const
{
	return operator==(Vector2<T>::zero);
}

template<typename T>
inline bool Vector2<T>::operator==(const Vector2<T>& vec) const
{
	if (!Math::Equals(x, vec.x)) return false;
	return Math::Equals(y, vec.y);
}

template<typename T>
inline bool Vector2<T>::operator!=(const Vector2<T>& vec) const
{
	return !operator==(vec);
}

template<typename T>
inline bool Vector2<T>::operator<(const Vector2<T>& vec) const
{
	if (x >= vec.x) return false;
	return (y < vec.y);
}

template<typename T>
inline bool Vector2<T>::operator<=(const Vector2<T>& vec) const
{
	if (x > vec.x) return false;
	return (y <= vec.y);
}

template<typename T>
inline bool Vector2<T>::operator>(const Vector2<T>& vec) const
{
	return !operator<=(vec);
}

template<typename T>
inline bool Vector2<T>::operator>=(const Vector2<T>& vec) const
{
	return !operator<(vec);
}

template<typename T>
inline bool Vector2<T>::Equals(const Vector2<T>& v1, const Vector2<T>& v2, const T& epsilon)
{
	if (!Math::Equals(v1.x, v2.x, epsilon)) return false;
	return Math::Equals(v1.y, v2.y, epsilon);
}

template<typename T>
inline T Vector2<T>::dotProduct(const Vector2<T>& v) const
{
	return x * v.x + y * v.y;
}

template<typename T>
inline T Vector2<T>::dotProduct(const Vector2<T>& v1, const Vector2<T>& v2)
{
	return v1.x * v2.x + v1.y * v2.y;
}

template<typename T>
inline T Vector2<T>::getSquaredLength() const
{
	return dotProduct(*this);
}

template<typename T>
inline T Vector2<T>::getLength() const
{
	return Math::Sqrt(getSquaredLength());
}

template<typename T>
inline Vector2<T>& Vector2<T>::setLength(const T& length, T* oldLength)
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
	return *this;
}

template<typename T>
inline Vector2<T>& Vector2<T>::normalize(T* oldLength)
{
	return setLength(T(1), oldLength);
}

template<typename T>
inline Vector2<T> Vector2<T>::normalized(T* oldLength) const
{
	return Vector2<T>(*this).normalize(oldLength);
}

template<typename T>
inline T Vector2<T>::getPolarAngle() const
{
	return Math::Atan2(x, y);
}

template<typename T>
inline Vector2<T>& Vector2<T>::setPolarAngle(T angle)
{
	const T length = getLength();
	x = Math::Cos(angle) * length;
	y = Math::Sin(angle) * length;
	return *this;
}

template<typename T>
inline Vector2<T>& Vector2<T>::rotate(T angle)
{
	const T c = Math::Cos(angle);
	const T s = Math::Sin(angle);
	const T tx = c * x - s * y;
	y = s * x + c * y;
	x = tx;
	return *this;
}

template<typename T>
inline Vector2<T> Vector2<T>::rotated(T angle) const
{
	const T c = Math::Cos(angle);
	const T s = Math::Sin(angle);
	return Vector2<T>(c * x - s * y, s * x + c * y);
}

template<typename T>
inline Vector2<T> Vector2<T>::lerp(const Vector2<T>& v1, const Vector2<T>& v2, const T& percent)
{
	const T one_minus_percent = T(1) - percent;
	return Vector2<T>(one_minus_percent * v1.x + percent * v2.x, one_minus_percent * v1.y + percent * v2.y);
}

template<typename T>
inline Vector2<T>& Vector2<T>::maximize(const Vector2<T>& v)
{
	if (v.x > x) x = v.x;
	if (v.y > y) y = v.y;
	return *this;
}

template<typename T>
inline Vector2<T> Vector2<T>::maximum(const Vector2<T>& v1, const Vector2<T>& v2)
{
	return Vector2<T>(Math::Max(v1.x, v2.x), Math::Max(v1.y, v2.y));
}

template<typename T>
inline Vector2<T>& Vector2<T>::minimize(const Vector2<T>& v)
{
	if (v.x < x) x = v.x;
	if (v.y < y) y = v.y;
	return *this;
}

template<typename T>
inline Vector2<T> Vector2<T>::minimum(const Vector2<T>& v1, const Vector2<T>& v2)
{
	return Vector2<T>(Math::Min(v1.x, v2.x), Math::Min(v1.y, v2.y));
}

template<typename T>
inline Vector2<T>& Vector2<T>::makeUnit()
{
	return set(T(1), T(1));
}

template<typename T>
inline Vector2<T>& Vector2<T>::makeUnitX()
{
	return set(T(1), T(0));
}

template<typename T>
inline Vector2<T>& Vector2<T>::makeUnitY()
{
	return set(T(0), T(1));
}

template<typename T>
inline Vector2<T>& Vector2<T>::makeZero()
{
	return set(T(0), T(0));
}

template<typename T>
inline Vector2<T>& Vector2<T>::makePolar(T angle, T length)
{
	return set(Math::Cos(angle) * length, Math::Sin(angle) * length);
}

template<typename T>
inline Vector2<T> Vector2<T>::polar(T angle, T length)
{
	return Vector2<T>(Math::Cos(angle) * length, Math::Sin(angle) * length);
}

template<typename T>
Vector2<T> operator+(const T & scale, const Vector2<T>& vector)
{
	return vector + scale;
}

template<typename T>
Vector2<T> operator-(const T& scale, const Vector2<T>& vector)
{
	return scale + (vector.operator-());
}

template<typename T>
inline Vector2<T> operator*(const T& scale, const Vector2<T>& vector)
{
	return vector * scale;
}

template<typename T>
Vector2<T> operator/(const T& scale, const Vector2<T>& vector)
{
	assert(vector[0] != T(0));
	assert(vector[1] != T(0));
	return Vector2<T>(scale / vector[0], scale / vector[1]);
}

template<typename T>
T dot(const Vector2<T>& v1, const Vector2<T>& v2)
{
	return v1.dotProduct(v2);
}

template<typename T>
Vector2<T> normalize(const Vector2<T>& vector, T* oldLength)
{
	return vector.normalized(oldLength);
}

template <typename T> const Vector2<T> Vector2<T>::unit(1, 1);
template <typename T> const Vector2<T> Vector2<T>::unitX(1, 0);
template <typename T> const Vector2<T> Vector2<T>::unitY(0, 1);
template <typename T> const Vector2<T> Vector2<T>::zero(0, 0);

typedef Vector2<Real> Vector2r;
typedef Vector2<F32> Vector2f;
typedef Vector2<I32> Vector2i;
typedef Vector2<U32> Vector2u;

typedef Vector2f vec2; // GLSL-like

} // namespace en