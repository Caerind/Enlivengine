#pragma once

#include <Enlivengine/Math/Vector3.hpp>

// TODO : constexpr

namespace en
{

template <typename T>
class Quaternion;

template <typename T>
class Matrix3
{
public:
	static constexpr I32 rows{ 3 };
	static constexpr I32 columns{ 3 };
	static constexpr I32 elements{ rows * columns };

	inline Matrix3();
	inline Matrix3(const Matrix3<T>& m);
	template <typename U>
	explicit inline Matrix3(const Matrix3<U>& m);
	explicit inline Matrix3(const T& s);
	explicit inline Matrix3(const T* a);
	inline Matrix3(const T& a11, const T& a21, const T& a31, const T& a12, const T& a22, const T& a32, const T& a13, const T& a23, const T& a33);
	~Matrix3() = default;

	inline Matrix3<T>& set(const Matrix3<T> & m);
	template <typename U>
	inline Matrix3<T>& set(const Matrix3<U> & m);
	inline Matrix3<T>& set(const T & s);
	inline Matrix3<T>& set(const T * a);
	inline Matrix3<T>& set(const T & a11, const T & a21, const T & a31, const T & a12, const T & a22, const T & a32, const T & a13, const T & a23, const T & a33);

	inline T& operator[](U32 i);
	inline const T& operator[](U32 i) const;
	inline T& operator()(U32 row, U32 column);
	inline const T& operator()(U32 row, U32 column) const;

	inline Vector3<T> getColumn(U32 i) const;
	inline Vector3<T> getRow(U32 j) const;
	inline Matrix3<T>& setColumn(U32 i, const Vector3<T>& column);
	inline Matrix3<T>& setRow(U32 j, const Vector3<T>& row);

	inline Matrix3<T>& operator=(const Matrix3<T> & m);
	inline const Matrix3<T>& operator+() const;
	inline Matrix3<T> operator-() const;

	inline Matrix3<T> operator+(const Matrix3<T> & m) const;
	inline Matrix3<T> operator-(const Matrix3<T> & m) const;
	inline Matrix3<T> operator*(const Matrix3<T> & m) const;
	inline Matrix3<T>& operator+=(const Matrix3<T> & m);
	inline Matrix3<T>& operator-=(const Matrix3<T> & m);
	inline Matrix3<T>& operator*=(const Matrix3<T> & m);

	inline Matrix3<T> operator+(const T & s) const;
	inline Matrix3<T> operator-(const T & s) const;
	inline Matrix3<T> operator*(const T & s) const;
	inline Matrix3<T> operator/(const T & s) const;
	inline Matrix3<T>& operator+=(const T & s);
	inline Matrix3<T>& operator-=(const T & s);
	inline Matrix3<T>& operator*=(const T & s);
	inline Matrix3<T>& operator/=(const T & s);

	inline bool operator==(const Matrix3<T> & m) const;
	inline bool operator!=(const Matrix3<T> & m) const;

	inline bool isIdentity() const;

	inline T getTrace() const;
	inline T getDeterminant() const;

	inline Matrix3<T>& inverse(bool* succeeded = nullptr);
	inline Matrix3<T> inversed(bool* succeeded = nullptr) const;

	inline Matrix3<T>& transpose();
	inline Matrix3<T> transposed() const;

	inline Quaternion<T> toQuaternion() const;

	inline Matrix3<T>& makeRotationX(const T & angle);
	inline Matrix3<T>& makeRotationX(const Vector2<T> & v);
	inline Matrix3<T>& makeRotationY(const T & angle);
	inline Matrix3<T>& makeRotationY(const Vector2<T> & v);
	inline Matrix3<T>& makeRotationZ(const T & angle);
	inline Matrix3<T>& makeRotationZ(const Vector2<T> & v);

	inline Matrix3<T>& makeZero();
	inline Matrix3<T>& makeIdentity();

	static inline Matrix3<T> rotationX(const T & angle);
	static inline Matrix3<T> rotationX(const Vector2<T> & v);
	static inline Matrix3<T> rotationY(const T & angle);
	static inline Matrix3<T> rotationY(const Vector2<T> & v);
	static inline Matrix3<T> rotationZ(const T & angle);
	static inline Matrix3<T> rotationZ(const Vector2<T> & v);

	static const Matrix3<T> zero;
	static const Matrix3<T> identity;

	T data[9];
};

template<typename T>
inline Matrix3<T>::Matrix3()
{
}

template<typename T>
inline Matrix3<T>::Matrix3(const Matrix3<T>& m)
{
	std::memcpy(data, m.data, 9 * sizeof(T));
}

template<typename T>
template<typename U>
inline Matrix3<T>::Matrix3(const Matrix3<U>& m)
{
	data[0] = T(m.data[0]);
	data[1] = T(m.data[1]);
	data[2] = T(m.data[2]);
	data[3] = T(m.data[3]);
	data[4] = T(m.data[4]);
	data[5] = T(m.data[5]);
	data[6] = T(m.data[6]);
	data[7] = T(m.data[7]);
	data[8] = T(m.data[8]);
}

template<typename T>
inline Matrix3<T>::Matrix3(const T& s)
{
	data[0] = s;
	data[1] = s;
	data[2] = s;
	data[3] = s;
	data[4] = s;
	data[5] = s;
	data[6] = s;
	data[7] = s;
	data[8] = s;
}

template<typename T>
inline Matrix3<T>::Matrix3(const T* a)
{
	std::memcpy(data, a, 9 * sizeof(T));
}

template<typename T>
inline Matrix3<T>::Matrix3(const T& a11, const T& a21, const T& a31, const T& a12, const T& a22, const T& a32, const T& a13, const T& a23, const T& a33)
{
	data[0] = a11;
	data[1] = a21;
	data[2] = a31;
	data[3] = a12;
	data[4] = a22;
	data[5] = a32;
	data[6] = a13;
	data[7] = a23;
	data[8] = a33;
}

template<typename T>
inline Matrix3<T>& Matrix3<T>::set(const Matrix3<T>& m)
{
	std::memcpy(data, m.data, 9 * sizeof(T));
	return *this;
}

template<typename T>
template<typename U>
inline Matrix3<T>& Matrix3<T>::set(const Matrix3<U>& m)
{
	data[0] = T(m.data[0]);
	data[1] = T(m.data[1]);
	data[2] = T(m.data[2]);
	data[3] = T(m.data[3]);
	data[4] = T(m.data[4]);
	data[5] = T(m.data[5]);
	data[6] = T(m.data[6]);
	data[7] = T(m.data[7]);
	data[8] = T(m.data[8]);
	return *this;
}

template<typename T>
inline Matrix3<T>& Matrix3<T>::set(const T& s)
{
	data[0] = s;
	data[1] = s;
	data[2] = s;
	data[3] = s;
	data[4] = s;
	data[5] = s;
	data[6] = s;
	data[7] = s;
	data[8] = s;
	return *this;
}

template<typename T>
inline Matrix3<T>& Matrix3<T>::set(const T* a)
{
	std::memcpy(data, a, 9 * sizeof(T));
	return *this;
}

template<typename T>
inline Matrix3<T>& Matrix3<T>::set(const T& a11, const T& a21, const T& a31, const T& a12, const T& a22, const T& a32, const T& a13, const T& a23, const T& a33)
{
	data[0] = a11;
	data[1] = a21;
	data[2] = a31;
	data[3] = a12;
	data[4] = a22;
	data[5] = a32;
	data[6] = a13;
	data[7] = a23;
	data[8] = a33;
	return *this;
}

template<typename T>
inline T& Matrix3<T>::operator[](U32 i)
{
	return data[i];
}

template<typename T>
inline const T& Matrix3<T>::operator[](U32 i) const
{
	return data[i];
}

template<typename T>
inline T& Matrix3<T>::operator()(U32 row, U32 column)
{
	return data[row + rows * column];
}

template<typename T>
inline const T& Matrix3<T>::operator()(U32 row, U32 column) const
{
	return data[row + rows * column];
}

template<typename T>
inline Vector3<T> Matrix3<T>::getColumn(U32 i) const
{
	return Vector3<T>(data[0 + rows * i], data[1 + rows * i], data[2 + rows * i]);
}

template<typename T>
inline Vector3<T> Matrix3<T>::getRow(U32 j) const
{
	return Vector3<T>(data[j], data[j + rows * 1], data[j + rows * 2]);
}

template<typename T>
inline Matrix3<T>& Matrix3<T>::setColumn(U32 i, const Vector3<T>& column)
{
	data[0 + rows * i] = column.x;
	data[1 + rows * i] = column.y;
	data[2 + rows * i] = column.z;
	return *this;
}

template<typename T>
inline Matrix3<T>& Matrix3<T>::setRow(U32 j, const Vector3<T>& row)
{
	data[j + rows * 0] = row.x;
	data[j + rows * 1] = row.y;
	data[j + rows * 2] = row.z;
	return *this;
}

template<typename T>
inline Matrix3<T>& Matrix3<T>::operator=(const Matrix3<T>& m)
{
	std::memcpy(data, m.data, sizeof(*this));
	return *this;
}

template<typename T>
inline const Matrix3<T>& Matrix3<T>::operator+() const
{
	return *this;
}

template<typename T>
inline Matrix3<T> Matrix3<T>::operator-() const
{
	return Matrix3<T>(-data[0], -data[1], -data[2], -data[3], -data[4], -data[5], -data[6], -data[7], -data[8]);
}

template<typename T>
inline Matrix3<T> Matrix3<T>::operator+(const Matrix3<T>& m) const
{
	return Matrix3<T>(data[0] + m.data[0], data[1] + m.data[1], data[2] + m.data[2], 
					  data[3] + m.data[3], data[4] + m.data[4], data[5] + m.data[5], 
					  data[6] + m.data[6], data[7] + m.data[7], data[8] + m.data[8]);
}

template<typename T>
inline Matrix3<T> Matrix3<T>::operator-(const Matrix3<T>& m) const
{
	return Matrix3<T>(data[0] - m.data[0], data[1] - m.data[1], data[2] - m.data[2],
					  data[3] - m.data[3], data[4] - m.data[4], data[5] - m.data[5],
				      data[6] - m.data[6], data[7] - m.data[7], data[8] - m.data[8]);
}

template<typename T>
inline Matrix3<T> Matrix3<T>::operator*(const Matrix3<T>& m) const
{
	Matrix3<T> out;
	Vector3<T> c0(m.getColumn(0));
	Vector3<T> c1(m.getColumn(1));
	Vector3<T> c2(m.getColumn(2));
	{
		Vector3<T> row(getRow(0));
		out.data[0] = c0.DotProduct(row);
		out.data[3] = c1.DotProduct(row);
		out.data[6] = c2.DotProduct(row);
	}
	{
		Vector3<T> row(getRow(1));
		out.data[1] = c0.DotProduct(row);
		out.data[4] = c1.DotProduct(row);
		out.data[7] = c2.DotProduct(row);
	}
	{
		Vector3<T> row(getRow(2));
		out.data[2] = c0.DotProduct(row);
		out.data[5] = c1.DotProduct(row);
		out.data[8] = c2.DotProduct(row);
	}
	return out;
}

template<typename T>
inline Matrix3<T>& Matrix3<T>::operator+=(const Matrix3<T>& m)
{
	data[0] += m.data[0];
	data[1] += m.data[1];
	data[2] += m.data[2];
	data[3] += m.data[3];
	data[4] += m.data[4];
	data[5] += m.data[5];
	data[6] += m.data[6];
	data[7] += m.data[7];
	data[8] += m.data[8];
	return *this;
}

template<typename T>
inline Matrix3<T>& Matrix3<T>::operator-=(const Matrix3<T>& m)
{
	data[0] -= m.data[0];
	data[1] -= m.data[1];
	data[2] -= m.data[2];
	data[3] -= m.data[3];
	data[4] -= m.data[4];
	data[5] -= m.data[5];
	data[6] -= m.data[6];
	data[7] -= m.data[7];
	data[8] -= m.data[8];
	return *this;
}

template<typename T>
inline Matrix3<T>& Matrix3<T>::operator*=(const Matrix3<T>& m)
{
	Vector3<T> r0(getRow(0));
	Vector3<T> r1(getRow(1));
	Vector3<T> r2(getRow(2));
	Vector3<T> c0(m.getColumn(0));
	Vector3<T> c1(m.getColumn(1));
	Vector3<T> c2(m.getColumn(2));
	data[0] = c0.DotProduct(r0);
	data[3] = c1.DotProduct(r0);
	data[6] = c2.DotProduct(r0);
	data[1] = c0.DotProduct(r1);
	data[4] = c1.DotProduct(r1);
	data[7] = c2.DotProduct(r1);
	data[2] = c0.DotProduct(r2);
	data[5] = c1.DotProduct(r2);
	data[8] = c2.DotProduct(r2);
	return *this;
}

template<typename T>
inline Matrix3<T> Matrix3<T>::operator+(const T& s) const
{
	return Matrix3<T>(data[0] + s, data[1] + s, data[2] + s, 
					  data[3] + s, data[4] + s, data[5] + s, 
					  data[6] + s, data[7] + s, data[8] + s);
}

template<typename T>
inline Matrix3<T> Matrix3<T>::operator-(const T& s) const
{
	return Matrix3<T>(data[0] - s, data[1] - s, data[2] - s,
					  data[3] - s, data[4] - s, data[5] - s,
					  data[6] - s, data[7] - s, data[8] - s);
}

template<typename T>
inline Matrix3<T> Matrix3<T>::operator*(const T& s) const
{
	return Matrix3<T>(data[0] * s, data[1] * s, data[2] * s,
					  data[3] * s, data[4] * s, data[5] * s,
					  data[6] * s, data[7] * s, data[8] * s);
}

template<typename T>
inline Matrix3<T> Matrix3<T>::operator/(const T& s) const
{
	enAssert(s != T(0));
	const T inv = 1 / s;
	return Matrix3<T>(data[0] * inv, data[1] * inv, data[2] * inv,
					  data[3] * inv, data[4] * inv, data[5] * inv,
					  data[6] * inv, data[7] * inv, data[8] * inv);
}

template<typename T>
inline Matrix3<T>& Matrix3<T>::operator+=(const T& s)
{
	data[0] += s;
	data[1] += s;
	data[2] += s;
	data[3] += s;
	data[4] += s;
	data[5] += s;
	data[6] += s;
	data[7] += s;
	data[8] += s;
	return *this;
}

template<typename T>
inline Matrix3<T>& Matrix3<T>::operator-=(const T& s)
{
	data[0] -= s;
	data[1] -= s;
	data[2] -= s;
	data[3] -= s;
	data[4] -= s;
	data[5] -= s;
	data[6] -= s;
	data[7] -= s;
	data[8] -= s;
	return *this;
}

template<typename T>
inline Matrix3<T>& Matrix3<T>::operator*=(const T& s)
{
	data[0] *= s;
	data[1] *= s;
	data[2] *= s;
	data[3] *= s;
	data[4] *= s;
	data[5] *= s;
	data[6] *= s;
	data[7] *= s;
	data[8] *= s;
	return *this;
}

template<typename T>
inline Matrix3<T>& Matrix3<T>::operator/=(const T& s)
{
	enAssert(s != T(0));
	const T inv = 1 / s;
	data[0] *= inv;
	data[1] *= inv;
	data[2] *= inv;
	data[3] *= inv;
	data[4] *= inv;
	data[5] *= inv;
	data[6] *= inv;
	data[7] *= inv;
	data[8] *= inv;
	return *this;
}

template<typename T>
inline bool Matrix3<T>::operator==(const Matrix3<T>& m) const
{
	if (!Math::Equals(data[0], m.data[0])) return false;
	if (!Math::Equals(data[1], m.data[1])) return false;
	if (!Math::Equals(data[2], m.data[2])) return false;
	if (!Math::Equals(data[3], m.data[3])) return false;
	if (!Math::Equals(data[4], m.data[4])) return false;
	if (!Math::Equals(data[5], m.data[5])) return false;
	if (!Math::Equals(data[6], m.data[6])) return false;
	if (!Math::Equals(data[7], m.data[7])) return false;
	return Math::Equals(data[8], m.data[8]);
}

template<typename T>
inline bool Matrix3<T>::operator!=(const Matrix3<T>& m) const
{
	return !operator==(m);
}

template<typename T>
inline bool Matrix3<T>::isIdentity() const
{
	return operator==(Matrix3<T>::identity);
}

template<typename T>
inline T Matrix3<T>::getTrace() const
{
	return data[0] + data[4] + data[8];
}

template<typename T>
inline T Matrix3<T>::getDeterminant() const
{
	const T det11 = data[4] * data[8] - data[5] * data[7];
	const T det12 = -data[1] * data[8] + data[2] * data[7];
	const T det13 = data[1] * data[5] - data[2] * data[4];
	return data[0] * det11 + data[3] * det12 + data[6] * det13;
}

template<typename T>
inline Matrix3<T>& Matrix3<T>::inverse(bool* succeeded)
{
	const T det11 = data[4] * data[8] - data[5] * data[7];
	const T det12 = -data[1] * data[8] + data[2] * data[7];
	const T det13 = data[1] * data[5] - data[2] * data[4];
	const T det = data[0] * det11 + data[3] * det12 + data[6] * det13;
	if (Math::Equals(det, T(0)))
	{
		if (succeeded != nullptr)
		{
			*succeeded = false;
		}
		return *this;
	}

	T inv[9];
	inv[0] = det11;
	inv[1] = det12;
	inv[2] = det13;
	inv[3] = data[6] * data[5] - data[3] * data[8];
	inv[4] = data[0] * data[8] - data[6] * data[2];
	inv[5] = data[3] * data[2] - data[0] * data[5];
	inv[6] = data[3] * data[7] - data[6] * data[4];
	inv[7] = data[6] * data[1] - data[0] * data[7];
	inv[8] = data[0] * data[4] - data[3] * data[1];

	const T invDet = 1 / det;
	for (int i = 0; i < 9; i++)
	{
		inv[i] *= invDet;
	}

	if (succeeded != nullptr)
	{
		*succeeded = true;
	}
	return set(inv);
}

template<typename T>
inline Matrix3<T> Matrix3<T>::inversed(bool* succeeded) const
{
	const T det11 = data[4] * data[8] - data[5] * data[7];
	const T det12 = -data[1] * data[8] + data[2] * data[7];
	const T det13 = data[1] * data[5] - data[2] * data[4];
	const T det = data[0] * det11 + data[3] * det12 + data[6] * det13;
	if (Math::Equals(det, T(0)))
	{
		if (succeeded != nullptr)
		{
			*succeeded = false;
		}
		return identity;
	}

	T inv[9];
	inv[0] = det11;
	inv[1] = det12;
	inv[2] = det13;
	inv[3] = data[6] * data[5] - data[3] * data[8];
	inv[4] = data[0] * data[8] - data[6] * data[2];
	inv[5] = data[3] * data[2] - data[0] * data[5];
	inv[6] = data[3] * data[7] - data[6] * data[4];
	inv[7] = data[6] * data[1] - data[0] * data[7];
	inv[8] = data[0] * data[4] - data[3] * data[1];

	const T invDet = 1 / det;
	for (int i = 0; i < 9; i++)
	{
		inv[i] *= invDet;
	}

	if (succeeded != nullptr)
	{
		*succeeded = true;
	}
	return Matrix3<T>(inv);
}

template<typename T>
inline Matrix3<T>& Matrix3<T>::transpose()
{
	std::swap(data[1], data[3]);
	std::swap(data[2], data[6]);
	std::swap(data[5], data[7]);
	return *this;
}

template<typename T>
inline Matrix3<T> Matrix3<T>::transposed() const
{
	return Matrix3<T>(data[0], data[3], data[6], data[1], data[4], data[7], data[2], data[5], data[8]);
}

template<typename T>
inline Quaternion<T> Matrix3<T>::toQuaternion() const
{
	return Quaternion<T>(*this);
}

template<typename T>
inline Matrix3<T>& Matrix3<T>::makeRotationX(const T& angle)
{
	return set(rotationX(angle));
}

template<typename T>
inline Matrix3<T>& Matrix3<T>::makeRotationX(const Vector2<T>& v)
{
	return set(rotationX(v));
}

template<typename T>
inline Matrix3<T>& Matrix3<T>::makeRotationY(const T& angle)
{
	return set(rotationY(angle));
}

template<typename T>
inline Matrix3<T>& Matrix3<T>::makeRotationY(const Vector2<T>& v)
{
	return set(rotationY(v));
}

template<typename T>
inline Matrix3<T>& Matrix3<T>::makeRotationZ(const T& angle)
{
	return set(rotationZ(angle));
}

template<typename T>
inline Matrix3<T>& Matrix3<T>::makeRotationZ(const Vector2<T>& v)
{
	return set(rotationZ(v));
}

template<typename T>
inline Matrix3<T>& Matrix3<T>::makeZero()
{
	return set(zero);
}

template<typename T>
inline Matrix3<T>& Matrix3<T>::makeIdentity()
{
	return set(identity);
}

template<typename T>
inline Matrix3<T> Matrix3<T>::rotationX(const T& angle)
{
	return rotationX(Vector2<T>(Math::Cos(angle), Math::Sin(angle)));
}

template<typename T>
inline Matrix3<T> Matrix3<T>::rotationX(const Vector2<T>& v)
{
	return Matrix3<T>(1, 0, 0, 0, v.x, v.y, 0, -v.y, v.x);
}

template<typename T>
inline Matrix3<T> Matrix3<T>::rotationY(const T & angle)
{
	return rotationY(Vector2<T>(Math::Cos(angle), Math::Sin(angle)));
}

template<typename T>
inline Matrix3<T> Matrix3<T>::rotationY(const Vector2<T>& v)
{
	return Matrix3<T>(v.x, 0, -v.y, 0, 1, 0, v.y, 0, v.x);
}

template<typename T>
inline Matrix3<T> Matrix3<T>::rotationZ(const T & angle)
{
	return rotationZ(Vector2<T>(Math::Cos(angle), Math::Sin(angle)));
}

template<typename T>
inline Matrix3<T> Matrix3<T>::rotationZ(const Vector2<T>& v)
{
	return Matrix3<T>(v.x, v.y, 0, -v.y, v.x, 0, 0, 0, 1);
}

template <typename T> const Matrix3<T> Matrix3<T>::zero(0, 0, 0, 0, 0, 0, 0, 0, 0);
template <typename T> const Matrix3<T> Matrix3<T>::identity(1, 0, 0, 0, 1, 0, 0, 0, 1);

typedef Matrix3<F32> Matrix3f;

typedef Matrix3f mat3; // GLSL-like

} // namespace en

ENLIVE_DEFINE_TYPE_INFO_TEMPLATE(en::Matrix3)
