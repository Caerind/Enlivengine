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

		inline Vector2<T>& set(const Vector2<T>& v);
		template <typename U>
		inline Vector2<T>& set(const Vector2<U>& v);
		inline Vector2<T>& set(const T& s);
		inline Vector2<T>& set(const T* a);
		inline Vector2<T>& set(const T& s1, const T& s2);

		inline T& operator()(U32 i);
		inline const T& operator()(U32 i) const;
		inline T& operator[](U32 i);
		inline const T& operator[](U32 i) const;

		inline Vector2<T>& operator=(const Vector2<T>& vec);
		inline const Vector2<T>& operator+() const;
		inline Vector2<T> operator-() const;

		inline Vector2<T> operator+(const Vector2<T>& vec) const;
		inline Vector2<T> operator-(const Vector2<T>& vec) const;
		inline Vector2<T> operator*(const Vector2<T>& vec) const;
		inline Vector2<T> operator/(const Vector2<T>& vec) const;
		inline Vector2<T>& operator+=(const Vector2<T>& vec);
		inline Vector2<T>& operator-=(const Vector2<T>& vec);
		inline Vector2<T>& operator*=(const Vector2<T>& vec);
		inline Vector2<T>& operator/=(const Vector2<T>& vec);

		inline Vector2<T> operator+(T scale) const;
		inline Vector2<T> operator-(T scale) const;
		inline Vector2<T> operator*(T scale) const;
		inline Vector2<T> operator/(T scale) const;
		inline Vector2<T>& operator+=(T scale);
		inline Vector2<T>& operator-=(T scale);
		inline Vector2<T>& operator*=(T scale);
		inline Vector2<T>& operator/=(T scale);

		inline bool isZero() const;
		inline bool operator==(const Vector2<T>& vec) const;
		inline bool operator!=(const Vector2<T>& vec) const;
		inline bool operator<(const Vector2<T>& vec) const;
		inline bool operator<=(const Vector2<T>& vec) const;
		inline bool operator>(const Vector2<T>& vec) const;
		inline bool operator>=(const Vector2<T>& vec) const;
		static inline bool Equals(const Vector2<T>& v1, const Vector2<T>& v2, const T& epsilon = std::numeric_limits<T>::epsilon());

		inline T dotProduct(const Vector2<T>& v) const;
		static inline T dotProduct(const Vector2<T>& v1, const Vector2<T>& v2);

		inline T getSquaredLength() const;
		inline T getLength() const;

		inline Vector2<T>& setLength(const T& length, T* oldLength = nullptr);

		inline Vector2<T>& normalize(T* oldLength = nullptr);
		inline Vector2<T> normalized(T* oldLength = nullptr) const;

		inline T getPolarAngle() const;
		inline Vector2<T>& setPolarAngle(T angle);

		inline Vector2<T>& rotate(T angle);
		inline Vector2<T> rotated(T angle) const;

		static inline Vector2<T> lerp(const Vector2<T>& v1, const Vector2<T>& v2, const T& percent);

		inline Vector2<T>& maximize(const Vector2<T>& v);
		static inline Vector2<T> maximum(const Vector2<T>& v1, const Vector2<T>& v2);

		inline Vector2<T>& minimize(const Vector2<T>& v);
		static inline Vector2<T> minimum(const Vector2<T>& v1, const Vector2<T>& v2);

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

		union 
		{
			T data[2];
			struct 
			{
				T x;
				T y;
			};
		};
};

template <typename T> Vector2<T> operator+(const T& scale, const Vector2<T>& vector);
template <typename T> Vector2<T> operator-(const T& scale, const Vector2<T>& vector);
template <typename T> Vector2<T> operator*(const T& scale, const Vector2<T>& vector);
template <typename T> Vector2<T> operator/(const T& scale, const Vector2<T>& vector);

template <typename T> T dot(const Vector2<T>& v1, const Vector2<T>& v2);

template <typename T> Vector2<T> normalize(const Vector2<T>& vector, T* oldLength = nullptr);

template<typename T>
inline Vector2<T>::Vector2()
{
}

template<typename T>
inline Vector2<T>::Vector2(const Vector2<T>& v)
{
	std::memcpy(data, v.data, 2 * sizeof(T));
}

template<typename T>
template<typename U>
inline Vector2<T>::Vector2(const Vector2<U>& v)
{
	data[0] = T(v.data[0]);
	data[1] = T(v.data[1]);
}

template<typename T>
inline Vector2<T>::Vector2(const T& s)
{
	data[0] = s;
	data[1] = s;
}

template<typename T>
inline Vector2<T>::Vector2(const T* a)
{
	std::memcpy(data, a, 2 * sizeof(T));
}

template<typename T>
inline Vector2<T>::Vector2(const T& s1, const T& s2)
{
	data[0] = s1;
	data[1] = s2;
}

template<typename T>
inline Vector2<T>& Vector2<T>::set(const Vector2<T>& v)
{
	std::memcpy(data, v.data, 2 * sizeof(T));
	return *this;
}

template<typename T>
template<typename U>
inline Vector2<T>& Vector2<T>::set(const Vector2<U>& v)
{
	data[0] = T(v.data[0]);
	data[1] = T(v.data[1]);
	return *this;
}

template<typename T>
inline Vector2<T>& Vector2<T>::set(const T& s)
{
	data[0] = s;
	data[1] = s;
	return *this;
}

template<typename T>
inline Vector2<T>& Vector2<T>::set(const T* a)
{
	std::memcpy(data, a, 2 * sizeof(T));
	return *this;
}

template<typename T>
inline Vector2<T>& Vector2<T>::set(const T& s1, const T& s2)
{
	data[0] = s1;
	data[1] = s2;
	return *this;
}

template<typename T>
inline T& Vector2<T>::operator()(U32 i)
{
	return data[i];
}

template<typename T>
inline const T& Vector2<T>::operator()(U32 i) const
{
	return data[i];
}

template<typename T>
inline T& Vector2<T>::operator[](U32 i)
{
	return data[i];
}

template<typename T>
inline const T& Vector2<T>::operator[](U32 i) const
{
	return data[i];
}

template<typename T>
inline Vector2<T>& Vector2<T>::operator=(const Vector2<T>& vec)
{
	std::memcpy(data, vec.data, 2 * sizeof(T));
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
	return Vector2<T>(-data[0], -data[1]);
}

template<typename T>
inline Vector2<T> Vector2<T>::operator+(const Vector2<T>& vec) const
{
	return Vector2<T>(data[0] + vec.data[0], data[1] + vec.data[1]);
}

template<typename T>
inline Vector2<T> Vector2<T>::operator-(const Vector2<T>& vec) const
{
	return Vector2<T>(data[0] - vec.data[0], data[1] - vec.data[1]);
}

template<typename T>
inline Vector2<T> Vector2<T>::operator*(const Vector2<T>& vec) const
{
	return Vector2<T>(data[0] * vec.data[0], data[1] * vec.data[1]);
}

template<typename T>
inline Vector2<T> Vector2<T>::operator/(const Vector2<T>& vec) const
{
	assert(vec.data[0] != T(0));
	assert(vec.data[1] != T(0));
	return Vector2<T>(data[0] / vec.data[0], data[1] / vec.data[1]);
}

template<typename T>
inline Vector2<T>& Vector2<T>::operator+=(const Vector2<T>& vec)
{
	data[0] += vec.data[0];
	data[1] += vec.data[1];
	return *this;
}

template<typename T>
inline Vector2<T>& Vector2<T>::operator-=(const Vector2<T>& vec)
{
	data[0] -= vec.data[0];
	data[1] -= vec.data[1];
	return *this;
}

template<typename T>
inline Vector2<T>& Vector2<T>::operator*=(const Vector2<T>& vec)
{
	data[0] *= vec.data[0];
	data[1] *= vec.data[1];
	return *this;
}

template<typename T>
inline Vector2<T>& Vector2<T>::operator/=(const Vector2<T>& vec)
{
	assert(vec.data[0] != T(0));
	assert(vec.data[1] != T(0));
	data[0] /= vec.data[0];
	data[1] /= vec.data[1];
	return *this;
}

template<typename T>
inline Vector2<T> Vector2<T>::operator+(T scale) const
{
	return Vector2<T>(data[0] + scale, data[1] + scale);
}

template<typename T>
inline Vector2<T> Vector2<T>::operator-(T scale) const
{
	return Vector2<T>(data[0] - scale, data[1] - scale);
}

template<typename T>
inline Vector2<T> Vector2<T>::operator*(T scale) const
{
	return Vector2<T>(data[0] * scale, data[1] * scale);
}

template<typename T>
inline Vector2<T> Vector2<T>::operator/(T scale) const
{
	assert(scale != T(0));
	const T inv = 1 / scale;
	return Vector2<T>(data[0] * inv, data[1] * inv);
}

template<typename T>
inline Vector2<T>& Vector2<T>::operator+=(T scale)
{
	data[0] += scale;
	data[1] += scale;
	return *this;
}

template<typename T>
inline Vector2<T>& Vector2<T>::operator-=(T scale)
{
	data[0] -= scale;
	data[1] -= scale;
	return *this;
}

template<typename T>
inline Vector2<T>& Vector2<T>::operator*=(T scale)
{
	data[0] *= scale;
	data[1] *= scale;
	return *this;
}

template<typename T>
inline Vector2<T>& Vector2<T>::operator/=(T scale)
{
	assert(scale != T(0));
	const T inv = 1 / scale;
	data[0] *= inv;
	data[1] *= inv;
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
	if (!Math::Equals(data[0], vec.data[0])) return false;
	return Math::Equals(data[1], vec.data[1]);
}

template<typename T>
inline bool Vector2<T>::operator!=(const Vector2<T>& vec) const
{
	return !operator==(vec);
}

template<typename T>
inline bool Vector2<T>::operator<(const Vector2<T>& vec) const
{
	if (data[0] >= vec.data[0]) return false;
	return (data[1] < vec.data[1]);
}

template<typename T>
inline bool Vector2<T>::operator<=(const Vector2<T>& vec) const
{
	if (data[0] > vec.data[0]) return false;
	return (data[1] <= vec.data[1]);
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
	if (!Math::Equals(v1.data[0], v2.data[0], epsilon)) return false;
	return Math::Equals(v1.data[1], v2.data[1], epsilon);
}

template<typename T>
inline T Vector2<T>::dotProduct(const Vector2<T>& v) const
{
	return data[0] * v.data[0] + data[1] * v.data[1];
}

template<typename T>
inline T Vector2<T>::dotProduct(const Vector2<T>& v1, const Vector2<T>& v2)
{
	return v1.data[0] * v2.data[0] + v1.data[1] * v2.data[1];
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
	data[0] *= factor;
	data[1] *= factor;
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
	return Vector2<T>(one_minus_percent * v1.data[0] + percent * v2.data[0], one_minus_percent * v1.data[1] + percent * v2.data[1]);
}

template<typename T>
inline Vector2<T>& Vector2<T>::maximize(const Vector2<T>& v)
{
	if (v.data[0] > data[0]) data[0] = v.data[0];
	if (v.data[1] > data[1]) data[1] = v.data[1];
	return *this;
}

template<typename T>
inline Vector2<T> Vector2<T>::maximum(const Vector2<T>& v1, const Vector2<T>& v2)
{
	return Vector2<T>(Math::Max(v1.data[0], v2.data[0]), Math::Max(v1.data[1], v2.data[1]));
}

template<typename T>
inline Vector2<T>& Vector2<T>::minimize(const Vector2<T>& v)
{
	if (v.data[0] < data[0]) data[0] = v.data[0];
	if (v.data[1] < data[1]) data[1] = v.data[1];
	return *this;
}

template<typename T>
inline Vector2<T> Vector2<T>::minimum(const Vector2<T>& v1, const Vector2<T>& v2)
{
	return Vector2<T>(Math::Min(v1.data[0], v2.data[0]), Math::Min(v1.data[1], v2.data[1]));
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