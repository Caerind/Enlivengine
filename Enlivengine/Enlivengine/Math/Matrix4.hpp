#pragma once

#include <Enlivengine/Math/Utilities.hpp>

// TODO : Matrix43 ?
// TODO : constexpr

namespace en
{

template <typename T> class Matrix3;
template <typename T> class Quaternion;
template <typename T> class Vector3;
template <typename T> class Vector4;

template <typename T>
class Matrix4
{
public:
	static constexpr I32 rows { 4 };
	static constexpr I32 columns { 4 };
	static constexpr I32 elements { rows * columns };

	inline Matrix4();
	inline Matrix4(const Matrix4<T>& m);
	template <typename U>
	explicit inline Matrix4(const Matrix4<U>& m);
	explicit inline Matrix4(const T& s);
	explicit inline Matrix4(const T* a);
	inline Matrix4(const Matrix3<T>& m);
	inline Matrix4(const T& a11, const T& a21, const T& a31, const T& a41, const T& a12, const T& a22, const T& a32, const T& a42, const T& a13, const T& a23, const T& a33, const T& a43, const T& a14, const T& a24, const T& a34, const T& a44);
	~Matrix4() = default;

	inline Matrix4<T>& set(const Matrix4<T>& m);
	template <typename U>
	inline Matrix4<T>& set(const Matrix4<U>& m);
	inline Matrix4<T>& set(const T& s);
	inline Matrix4<T>& set(const T* a);
	inline Matrix4<T>& set(const Matrix3<T>& m);
	inline Matrix4<T>& set(const T& a11, const T& a21, const T& a31, const T& a41, const T& a12, const T& a22, const T& a32, const T& a42, const T& a13, const T& a23, const T& a33, const T& a43, const T& a14, const T& a24, const T& a34, const T& a44);
		
	inline T& operator[](U32 i);
	inline const T& operator[](U32 i) const;
	inline T& operator()(U32 row, U32 column);
	inline const T& operator()(U32 row, U32 column) const;

	inline Vector4<T> getColumn(U32 i) const;
	inline Vector4<T> getRow(U32 j) const;
	inline Matrix4<T>& setColumn(U32 i, const Vector4<T>& column);
	inline Matrix4<T>& setRow(U32 j, const Vector4<T>& row);

	inline Matrix4<T>& operator=(const Matrix4<T>& m);
	inline const Matrix4<T>& operator+() const;
	inline Matrix4<T> operator-() const;

	inline Matrix4<T> operator+(const Matrix4<T>& m) const;
	inline Matrix4<T> operator-(const Matrix4<T>& m) const;
	inline Matrix4<T> operator*(const Matrix4<T>& m) const;
	inline Matrix4<T>& operator+=(const Matrix4<T>& m);
	inline Matrix4<T>& operator-=(const Matrix4<T>& m);
	inline Matrix4<T>& operator*=(const Matrix4<T>& m);

	inline Matrix4<T> operator+(const T& s) const;
	inline Matrix4<T> operator-(const T& s) const;
	inline Matrix4<T> operator*(const T& s) const;
	inline Matrix4<T> operator/(const T& s) const;
	inline Matrix4<T>& operator+=(const T& s);
	inline Matrix4<T>& operator-=(const T& s);
	inline Matrix4<T>& operator*=(const T& s);
	inline Matrix4<T>& operator/=(const T& s);

	inline Vector3<T> operator*(const Vector3<T>& vector) const; // Representing a point, not a direction (w = 1)
	inline Vector4<T> operator*(const Vector4<T>& vector) const;
		
	inline Vector3<T> transformPosition(const Vector3<T>& position) const; // w = 1
	inline Vector3<T> transformDirection(const Vector3<T>& direction) const; // w = 0

	inline bool operator==(const Matrix4<T>& m) const;
	inline bool operator!=(const Matrix4<T>& m) const;

	inline Matrix4<T>& fromMatrix3(const Matrix3<T>& matrix);
	inline void toMatrix3(Matrix3<T>& matrix) const;
	inline Matrix3<T> toMatrix3() const;

	inline Quaternion<T> getRotation() const;
	inline Vector3<T> getScale() const;
	inline Vector3<T> getTranslation() const;

	inline bool hasScale() const;
	inline bool isAffine() const;
	inline bool isIdentity() const;

	inline T getTrace() const;
	inline T getDeterminant() const;

	inline Matrix4<T>& inverse(bool* succeeded = nullptr);
	inline Matrix4<T> inversed(bool* succeeded = nullptr) const;

	inline Matrix4<T>& transpose();
	inline Matrix4<T> transposed() const;

	inline Matrix4<T>& setRotation(const Quaternion<T>& rotation);
	inline Matrix4<T>& setRotation(const Matrix3<T>& rotation);
	inline Matrix4<T>& setScale(const Vector3<T>& scale);
	inline Matrix4<T>& setScale(const T& sx, const T& sy, const T& sz);
	inline Matrix4<T>& setScale(const T& s);
	inline Matrix4<T>& setTranslation(const Vector3<T>& translation);
	inline Matrix4<T>& setTranslation(const T& tx, const T& ty, const T& tz);

	inline Matrix4<T>& applyRotation(const Quaternion<T>& rotation);
	inline Matrix4<T>& applyRotation(const Matrix3<T>& rotation);
	inline Matrix4<T>& applyScale(const Vector3<T>& scale);
	inline Matrix4<T>& applyScale(const T& sx, const T& sy, const T& sz);
	inline Matrix4<T>& applyScale(const T& s);
	inline Matrix4<T>& applyTranslation(const Vector3<T>& translation);
	inline Matrix4<T>& applyTranslation(const T& tx, const T& ty, const T& tz);
	// TODO : ApplyTransformation

	inline Matrix4<T> rotated(const Quaternion<T>& rotation);
	inline Matrix4<T> rotated(const Matrix3<T>& rotation);
	inline Matrix4<T> scaled(const Vector3<T>& scale);
	inline Matrix4<T> scaled(const T& sx, const T& sy, const T& sz);
	inline Matrix4<T> scaled(const T& s);
	inline Matrix4<T> translated(const Vector3<T>& translation);
	inline Matrix4<T> translated(const T& tx, const T& ty, const T& tz);
	inline Matrix4<T> transformed(const Vector3<T>& translation, const Quaternion<T>& rotation);
	inline Matrix4<T> transformed(const Vector3<T>& translation, const Quaternion<T>& rotation, const Vector3<T>& scale);

	inline Matrix4<T>& makeRotation(const Quaternion<T>& rotation);
	inline Matrix4<T>& makeRotation(const Matrix3<T>& rotation);
	inline Matrix4<T>& makeScale(const Vector3<T>& scale);
	inline Matrix4<T>& makeTranslation(const Vector3<T>& translation);
	inline Matrix4<T>& makeTransform(const Vector3<T>& translation, const Quaternion<T>& rotation);
	inline Matrix4<T>& makeTransform(const Vector3<T>& translation, const Quaternion<T>& rotation, const Vector3<T>& scale);

	inline Matrix4<T>& makeViewMatrix(const Vector3<T>& translation, const Quaternion<T>& rotation);
	inline Matrix4<T>& makeLookAt(const Vector3<T>& eye, const Vector3<T>& target, const Vector3<T>& up = Vector3<T>::up);
	inline Matrix4<T>& makeOrtho(const T& left, const T& right, const T& top, const T& bottom, const T& zNear, const T& zFar);
	inline Matrix4<T>& makePerspective(const T& fov, const T& ratio, const T& zNear, const T& zFar);

	inline Matrix4<T>& makeZero();
	inline Matrix4<T>& makeIdentity();

	static inline Matrix4<T> rotation(const Quaternion<T>& rotation);
	static inline Matrix4<T> rotation(const Matrix3<T>& rotation);
	static inline Matrix4<T> scale(const Vector3<T>& scale);
	static inline Matrix4<T> scale(const T& sx, const T& sy, const T& sz);
	static inline Matrix4<T> scale(const T& s);
	static inline Matrix4<T> translation(const Vector3<T>& translation);
	static inline Matrix4<T> translation(const T& tx, const T& ty, const T& tz);
	static inline Matrix4<T> transform(const Vector3<T>& translation, const Quaternion<T>& rotation);
	static inline Matrix4<T> transform(const Vector3<T>& translation, const Quaternion<T>& rotation, const Vector3<T>& scale);

	static inline Matrix4<T> viewMatrix(const Vector3<T>& translation, const Quaternion<T>& rotation);
	static inline Matrix4<T> lookAt(const Vector3<T>& eye, const Vector3<T>& target, const Vector3<T>& up = Vector3<T>::up);
	static inline Matrix4<T> ortho(const T& left, const T& right, const T& top, const T& bottom, const T& zNear, const T& zFar);
	static inline Matrix4<T> perspective(const T& fov, const T& ratio, const T& zNear, const T& zFar);

	static const Matrix4<T> zero;
	static const Matrix4<T> identity;

	T data[16];
};

} // namespace en

#include <Enlivengine/Math/Matrix3.hpp>
#include <Enlivengine/Math/Vector4.hpp>
#include <Enlivengine/Math/Quaternion.hpp>

namespace en
{

template<typename T>
inline Matrix4<T>::Matrix4()
{
}

template<typename T>
inline Matrix4<T>::Matrix4(const Matrix4<T>& m)
{
	std::memcpy(data, m.data, 16 * sizeof(T));
}

template<typename T>
template<typename U>
inline Matrix4<T>::Matrix4(const Matrix4<U>& m)
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
	data[9] = T(m.data[9]);
	data[10] = T(m.data[10]);
	data[11] = T(m.data[11]);
	data[12] = T(m.data[12]);
	data[13] = T(m.data[13]);
	data[14] = T(m.data[14]);
	data[15] = T(m.data[15]);
}

template<typename T>
inline Matrix4<T>::Matrix4(const T& s)
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
	data[9] = s;
	data[10] = s;
	data[11] = s;
	data[12] = s;
	data[13] = s;
	data[14] = s;
	data[15] = s;
}

template<typename T>
inline Matrix4<T>::Matrix4(const T* a)
{
	std::memcpy(data, a, 16 * sizeof(T));
}

template<typename T>
inline Matrix4<T>::Matrix4(const Matrix3<T>& m)
{
	data[0] = m.data[0];
	data[1] = m.data[1];
	data[2] = m.data[2];
	data[3] = 0;
	data[4] = m.data[3];
	data[5] = m.data[4];
	data[6] = m.data[5];
	data[7] = 0;
	data[8] = m.data[6];
	data[9] = m.data[7];
	data[10] = m.data[8];
	data[11] = 0;
	data[12] = 0;
	data[13] = 0;
	data[14] = 0;
	data[15] = 1;
}

template<typename T>
inline Matrix4<T>::Matrix4(const T& a11, const T& a21, const T& a31, const T& a41, const T& a12, const T& a22, const T& a32, const T& a42, const T& a13, const T& a23, const T& a33, const T& a43, const T& a14, const T& a24, const T& a34, const T& a44)
{
	data[0] = a11;
	data[1] = a21;
	data[2] = a31;
	data[3] = a41;
	data[4] = a12;
	data[5] = a22;
	data[6] = a32;
	data[7] = a42;
	data[8] = a13;
	data[9] = a23;
	data[10] = a33;
	data[11] = a43;
	data[12] = a14;
	data[13] = a24;
	data[14] = a34;
	data[15] = a44;
}

template<typename T>
inline Matrix4<T>& Matrix4<T>::set(const Matrix4<T>& m)
{
	std::memcpy(data, m.data, 16 * sizeof(T));
	return *this;
}

template<typename T>
template<typename U>
inline Matrix4<T>& Matrix4<T>::set(const Matrix4<U>& m)
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
	data[9] = T(m.data[9]);
	data[10] = T(m.data[10]);
	data[11] = T(m.data[11]);
	data[12] = T(m.data[12]);
	data[13] = T(m.data[13]);
	data[14] = T(m.data[14]);
	data[15] = T(m.data[15]);
	return *this;
}

template<typename T>
inline Matrix4<T>& Matrix4<T>::set(const T& s)
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
	data[9] = s;
	data[10] = s;
	data[11] = s;
	data[12] = s;
	data[13] = s;
	data[14] = s;
	data[15] = s;
	return *this;
}

template<typename T>
inline Matrix4<T>& Matrix4<T>::set(const T* a)
{
	std::memcpy(data, a, 16 * sizeof(T));
	return *this;
}

template<typename T>
inline Matrix4<T>& Matrix4<T>::set(const Matrix3<T>& m)
{
	data[0] = m.data[0];
	data[1] = m.data[1];
	data[2] = m.data[2];
	data[3] = 0;
	data[4] = m.data[3];
	data[5] = m.data[4];
	data[6] = m.data[5];
	data[7] = 0;
	data[8] = m.data[6];
	data[9] = m.data[7];
	data[10] = m.data[8];
	data[11] = 0;
	data[12] = 0;
	data[13] = 0;
	data[14] = 0;
	data[15] = 1;
	return *this;
}

template<typename T>
inline Matrix4<T>& Matrix4<T>::set(const T& a11, const T& a21, const T& a31, const T& a41, const T& a12, const T& a22, const T& a32, const T& a42, const T& a13, const T& a23, const T& a33, const T& a43, const T& a14, const T& a24, const T& a34, const T& a44)
{
	data[0] = a11;
	data[1] = a21;
	data[2] = a31;
	data[3] = a41;
	data[4] = a12;
	data[5] = a22;
	data[6] = a32;
	data[7] = a42;
	data[8] = a13;
	data[9] = a23;
	data[10] = a33;
	data[11] = a43;
	data[12] = a14;
	data[13] = a24;
	data[14] = a34;
	data[15] = a44;
	return *this;
}

template<typename T>
inline T& Matrix4<T>::operator[](U32 i)
{
	return data[i];
}

template<typename T>
inline const T& Matrix4<T>::operator[](U32 i) const
{
	return data[i];
}

template<typename T>
inline T& Matrix4<T>::operator()(U32 row, U32 column)
{
	return data[row + rows * column];
}

template<typename T>
inline const T& Matrix4<T>::operator()(U32 row, U32 column) const
{
	return data[row + rows * column];
}

template<typename T>
inline Vector4<T> Matrix4<T>::getColumn(U32 i) const
{
	return Vector4<T>(data[0 + rows * i], data[1 + rows * i], data[2 + rows * i], data[3 + rows * i]);
}

template<typename T>
inline Vector4<T> Matrix4<T>::getRow(U32 j) const
{
	return Vector4<T>(data[j + rows * 0], data[j + rows * 1], data[j + rows * 2], data[j + rows * 3]);
}

template<typename T>
inline Matrix4<T>& Matrix4<T>::setColumn(U32 i, const Vector4<T>& column)
{
	data[0 + rows * i] = column.x;
	data[1 + rows * i] = column.y;
	data[2 + rows * i] = column.z;
	data[3 + rows * i] = column.w;
	return *this;
}

template<typename T>
inline Matrix4<T>& Matrix4<T>::setRow(U32 j, const Vector4<T>& row)
{
	data[j + rows * 0] = row.x;
	data[j + rows * 1] = row.y;
	data[j + rows * 2] = row.z;
	data[j + rows * 3] = row.w;
	return *this;
}

template<typename T>
inline Matrix4<T>& Matrix4<T>::operator=(const Matrix4<T>& m)
{
	std::memcpy(data, m.data, sizeof(*this));
	return *this;
}

template<typename T>
inline const Matrix4<T>& Matrix4<T>::operator+() const
{
	return *this;
}

template<typename T>
inline Matrix4<T> Matrix4<T>::operator-() const
{
	return Matrix4<T>(-data[0], -data[1], -data[2], -data[3], -data[4], -data[5], -data[6], -data[7], -data[8], -data[9], -data[10], -data[11], -data[12], -data[13], -data[14], -data[15]);
}

template<typename T>
inline Matrix4<T> Matrix4<T>::operator+(const Matrix4<T>& m) const
{
	return Matrix4<T>(data[0] + m.data[0], data[1] + m.data[1], data[2] + m.data[2], data[3] + m.data[3],
		data[4] + m.data[4], data[5] + m.data[5], data[6] + m.data[6], data[7] + m.data[7],
		data[8] + m.data[8], data[9] + m.data[9], data[10] + m.data[10], data[11] + m.data[11],
		data[12] + m.data[12], data[13] + m.data[13], data[14] + m.data[14], data[15] + m.data[15]);
}

template<typename T>
inline Matrix4<T> Matrix4<T>::operator-(const Matrix4<T>& m) const
{
	return Matrix4<T>(data[0] - m.data[0], data[1] - m.data[1], data[2] - m.data[2], data[3] - m.data[3],
		data[4] - m.data[4], data[5] - m.data[5], data[6] - m.data[6], data[7] - m.data[7],
		data[8] - m.data[8], data[9] - m.data[9], data[10] - m.data[10], data[11] - m.data[11],
		data[12] - m.data[12], data[13] - m.data[13], data[14] - m.data[14], data[15] - m.data[15]);
}

template<typename T>
inline Matrix4<T> Matrix4<T>::operator*(const Matrix4<T>& m) const
{
	Matrix4<T> out;
	Vector4<T> c0(m.getColumn(0));
	Vector4<T> c1(m.getColumn(1));
	Vector4<T> c2(m.getColumn(2));
	Vector4<T> c3(m.getColumn(3));
	{
		Vector4<T> row(getRow(0));
		out.data[0] = c0.DotProduct(row);
		out.data[4] = c1.DotProduct(row);
		out.data[8] = c2.DotProduct(row);
		out.data[12] = c3.DotProduct(row);
	}
	{
		Vector4<T> row(getRow(1));
		out.data[1] = c0.DotProduct(row);
		out.data[5] = c1.DotProduct(row);
		out.data[9] = c2.DotProduct(row);
		out.data[13] = c3.DotProduct(row);
	}
	{
		Vector4<T> row(getRow(2));
		out.data[2] = c0.DotProduct(row);
		out.data[6] = c1.DotProduct(row);
		out.data[10] = c2.DotProduct(row);
		out.data[14] = c3.DotProduct(row);
	}
	{
		Vector4<T> row(getRow(3));
		out.data[3] = c0.DotProduct(row);
		out.data[7] = c1.DotProduct(row);
		out.data[11] = c2.DotProduct(row);
		out.data[15] = c3.DotProduct(row);
	}
	return out;
}

template<typename T>
inline Matrix4<T>& Matrix4<T>::operator+=(const Matrix4<T>& m)
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
	data[9] += m.data[9];
	data[10] += m.data[10];
	data[11] += m.data[11];
	data[12] += m.data[12];
	data[13] += m.data[13];
	data[14] += m.data[14];
	data[15] += m.data[15];
	return *this;
}

template<typename T>
inline Matrix4<T>& Matrix4<T>::operator-=(const Matrix4<T>& m)
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
	data[9] -= m.data[9];
	data[10] -= m.data[10];
	data[11] -= m.data[11];
	data[12] -= m.data[12];
	data[13] -= m.data[13];
	data[14] -= m.data[14];
	data[15] -= m.data[15];
	return *this;
}

template<typename T>
inline Matrix4<T>& Matrix4<T>::operator*=(const Matrix4<T>& m)
{
	Vector4<T> r0(getRow(0));
	Vector4<T> r1(getRow(1));
	Vector4<T> r2(getRow(2));
	Vector4<T> r3(getRow(3));
	Vector4<T> c0(m.getColumn(0));
	Vector4<T> c1(m.getColumn(1));
	Vector4<T> c2(m.getColumn(2));
	Vector4<T> c3(m.getColumn(3));
	data[0] = c0.DotProduct(r0);
	data[4] = c1.DotProduct(r0);
	data[8] = c2.DotProduct(r0);
	data[12] = c3.DotProduct(r0);
	data[1] = c0.DotProduct(r1);
	data[5] = c1.DotProduct(r1);
	data[9] = c2.DotProduct(r1);
	data[13] = c3.DotProduct(r1);
	data[2] = c0.DotProduct(r2);
	data[6] = c1.DotProduct(r2);
	data[10] = c2.DotProduct(r2);
	data[14] = c3.DotProduct(r2);
	data[3] = c0.DotProduct(r3);
	data[7] = c1.DotProduct(r3);
	data[11] = c2.DotProduct(r3);
	data[15] = c3.DotProduct(r3);
	return *this;
}

template<typename T>
inline Matrix4<T> Matrix4<T>::operator+(const T& s) const
{
	return Matrix4<T>(data[0] + s, data[1] + s, data[2] + s, data[3] + s,
		data[4] + s, data[5] + s, data[6] + s, data[7] + s,
		data[8] + s, data[9] + s, data[10] + s, data[11] + s,
		data[12] + s, data[13] + s, data[14] + s, data[15] + s);
}

template<typename T>
inline Matrix4<T> Matrix4<T>::operator-(const T& s) const
{
	return Matrix4<T>(data[0] - s, data[1] - s, data[2] - s, data[3] - s,
		data[4] - s, data[5] - s, data[6] - s, data[7] - s,
		data[8] - s, data[9] - s, data[10] - s, data[11] - s,
		data[12] - s, data[13] - s, data[14] - s, data[15] - s);
}

template<typename T>
inline Matrix4<T> Matrix4<T>::operator*(const T& s) const
{
	return Matrix4<T>(data[0] * s, data[1] * s, data[2] * s, data[3] * s,
		data[4] * s, data[5] * s, data[6] * s, data[7] * s,
		data[8] * s, data[9] * s, data[10] * s, data[11] * s,
		data[12] * s, data[13] * s, data[14] * s, data[15] * s);
}

template<typename T>
inline Matrix4<T> Matrix4<T>::operator/(const T& s) const
{
	enAssert(s != T(0));
	const T inv = 1 / s;
	return Matrix4<T>(data[0] * inv, data[1] * inv, data[2] * inv, data[3] * inv,
		data[4] * inv, data[5] * inv, data[6] * inv, data[7] * inv,
		data[8] * inv, data[9] * inv, data[10] * inv, data[11] * inv,
		data[12] * inv, data[13] * inv, data[14] * inv, data[15] * inv);
}

template<typename T>
inline Matrix4<T>& Matrix4<T>::operator+=(const T& s)
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
	data[9] += s;
	data[10] += s;
	data[11] += s;
	data[12] += s;
	data[13] += s;
	data[14] += s;
	data[15] += s;
	return *this;
}

template<typename T>
inline Matrix4<T>& Matrix4<T>::operator-=(const T& s)
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
	data[9] -= s;
	data[10] -= s;
	data[11] -= s;
	data[12] -= s;
	data[13] -= s;
	data[14] -= s;
	data[15] -= s;
	return *this;
}

template<typename T>
inline Matrix4<T>& Matrix4<T>::operator*=(const T& s)
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
	data[9] *= s;
	data[10] *= s;
	data[11] *= s;
	data[12] *= s;
	data[13] *= s;
	data[14] *= s;
	data[15] *= s;
	return *this;
}

template<typename T>
inline Matrix4<T>& Matrix4<T>::operator/=(const T& s)
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
	data[9] *= inv;
	data[10] *= inv;
	data[11] *= inv;
	data[12] *= inv;
	data[13] *= inv;
	data[14] *= inv;
	data[15] *= inv;
	return *this;
}

template<typename T>
inline Vector3<T> Matrix4<T>::operator*(const Vector3<T>& vector) const
{
	return Vector3<T>(vector.x * data[0] + vector.y * data[4] + vector.z * data[8] + data[12],
		vector.x * data[1] + vector.y * data[5] + vector.z * data[9] + data[13],
		vector.x * data[2] + vector.y * data[6] + vector.z * data[10] + data[14]);
}

template<typename T>
inline Vector4<T> Matrix4<T>::operator*(const Vector4<T>& vector) const
{
	return Vector4<T>(vector.x * data[0] + vector.y * data[4] + vector.z * data[8] + vector.w * data[12],
		vector.x * data[1] + vector.y * data[5] + vector.z * data[9] + vector.w * data[13],
		vector.x * data[2] + vector.y * data[6] + vector.z * data[10] + vector.w * data[14],
		vector.x * data[3] + vector.y * data[7] + vector.z * data[11] + vector.w * data[15]);
}

template<typename T>
inline Vector3<T> Matrix4<T>::transformPosition(const Vector3<T>& position) const
{
	return Vector3<T>(position.x * data[0] + position.y * data[4] + position.z * data[8] + data[12],
		position.x * data[1] + position.y * data[5] + position.z * data[9] + data[13],
		position.x * data[2] + position.y * data[6] + position.z * data[10] + data[14]);
}

template<typename T>
inline Vector3<T> Matrix4<T>::transformDirection(const Vector3<T>& direction) const
{
	return Vector3<T>(direction.x * data[0] + direction.y * data[4] + direction.z * data[8],
		direction.x * data[1] + direction.y * data[5] + direction.z * data[9],
		direction.x * data[2] + direction.y * data[6] + direction.z * data[10]);
}

template<typename T>
inline bool Matrix4<T>::operator==(const Matrix4<T>& m) const
{
	if (!Math::Equals(data[0], m.data[0])) return false;
	if (!Math::Equals(data[1], m.data[1])) return false;
	if (!Math::Equals(data[2], m.data[2])) return false;
	if (!Math::Equals(data[3], m.data[3])) return false;
	if (!Math::Equals(data[4], m.data[4])) return false;
	if (!Math::Equals(data[5], m.data[5])) return false;
	if (!Math::Equals(data[6], m.data[6])) return false;
	if (!Math::Equals(data[7], m.data[7])) return false;
	if (!Math::Equals(data[8], m.data[8])) return false;
	if (!Math::Equals(data[9], m.data[9])) return false;
	if (!Math::Equals(data[10], m.data[10])) return false;
	if (!Math::Equals(data[11], m.data[11])) return false;
	if (!Math::Equals(data[12], m.data[12])) return false;
	if (!Math::Equals(data[13], m.data[13])) return false;
	if (!Math::Equals(data[14], m.data[14])) return false;
	return Math::Equals(data[15], m.data[15]);
}

template<typename T>
inline bool Matrix4<T>::operator!=(const Matrix4<T>& m) const
{
	return !operator==(m);
}

template<typename T>
inline Matrix4<T>& Matrix4<T>::fromMatrix3(const Matrix3<T>& matrix)
{
	data[0] = matrix[0];
	data[1] = matrix[1];
	data[2] = matrix[2];
	data[3] = 0;
	data[4] = matrix[3];
	data[5] = matrix[4];
	data[6] = matrix[5];
	data[7] = 0;
	data[8] = matrix[6];
	data[9] = matrix[7];
	data[10] = matrix[8];
	data[11] = 0;
	data[12] = 0;
	data[13] = 0;
	data[14] = 0;
	data[15] = 1;
	return *this;
}

template<typename T>
inline void Matrix4<T>::toMatrix3(Matrix3<T>& matrix) const
{
	matrix[0] = data[0];
	matrix[1] = data[1];
	matrix[2] = data[2];
	matrix[3] = data[4];
	matrix[4] = data[5];
	matrix[5] = data[6];
	matrix[6] = data[8];
	matrix[7] = data[9];
	matrix[8] = data[10];
}

template<typename T>
inline Matrix3<T> Matrix4<T>::toMatrix3() const
{
	return Matrix3<T>(data[0], data[1], data[2], data[4], data[5], data[6], data[8], data[9], data[10]);
}

template<typename T>
inline Quaternion<T> Matrix4<T>::getRotation() const
{
	Matrix3<T> rot;
	Vector3<T> scale = getScale();
	rot[0] = data[0] / scale.x;
	rot[1] = data[1] / scale.x;
	rot[2] = data[2] / scale.x;
	rot[3] = data[4] / scale.y;
	rot[4] = data[5] / scale.y;
	rot[5] = data[6] / scale.y;
	rot[6] = data[8] / scale.z;
	rot[7] = data[9] / scale.z;
	rot[8] = data[10] / scale.z;
	return Quaternion<T>(rot);
}

template<typename T>
inline Vector3<T> Matrix4<T>::getScale() const
{
	return Vector3<T>(Math::Sqrt(data[0] * data[0] + data[1] * data[1] + data[2] * data[2]),
		Math::Sqrt(data[4] * data[4] + data[5] * data[5] + data[6] * data[6]),
		Math::Sqrt(data[8] * data[8] + data[9] * data[9] + data[10] * data[10]));
}

template<typename T>
inline Vector3<T> Matrix4<T>::getTranslation() const
{
	return Vector3<T>(data[12], data[13], data[14]);
}

template<typename T>
inline bool Matrix4<T>::hasScale() const
{
	if (!Math::Equals(data[0] * data[0] + data[1] * data[1] + data[2] * data[2], T(1))) return true;
	if (!Math::Equals(data[4] * data[4] + data[5] * data[5] + data[6] * data[6], T(1))) return true;
	if (!Math::Equals(data[8] * data[8] + data[9] * data[9] + data[10] * data[10], T(1))) return true;
	return false;
}

template<typename T>
inline bool Matrix4<T>::isAffine() const
{
	if (!Math::Equals(data[3], T(0))) return false;
	if (!Math::Equals(data[7], T(0))) return false;
	if (!Math::Equals(data[11], T(0))) return false;
	return Math::Equals(data[15], T(1));
}

template<typename T>
inline bool Matrix4<T>::isIdentity() const
{
	return operator==(Matrix4<T>::identity);
}

template<typename T>
inline T Matrix4<T>::getTrace() const
{
	return data[0] + data[5] + data[10] + data[15];
}

template<typename T>
inline T Matrix4<T>::getDeterminant() const
{
	// We will use the last row of the matrix
	// Because if the matrix is affine it will be faster

	// Compute det2x2
	const T m22m33 = data[5] * data[10] - data[6] * data[9];
	const T m21m33 = data[1] * data[10] - data[2] * data[6];
	const T m21m32 = data[1] * data[9] - data[2] * data[5];

	// Compute det3x3
	const T m11m33 = data[0] * m22m33 - data[4] * m21m33 + data[8] * m21m32;
	if (isAffine())
	{
		// If affine, the last row is full of zero except the last one
		// Return the det of the 3x3
		return m11m33; // 9 mul, 5 add
	}
	else
	{
		// Compute det2x2, each det2x2 is used 2 times for the det3
		const T m21m34 = data[1] * data[14] - data[2] * data[9];
		const T m22m34 = data[5] * data[14] - data[6] * data[13];
		const T m23m34 = data[9] * data[14] - data[10] * data[13];

		// Compute det3x3
		const T m12m34 = data[4] * m23m34 - data[8] * m22m34 + data[12] * m22m33;
		const T m11m34 = data[0] * m23m34 - data[8] * m21m34 + data[12] * m21m33;
		const T m11m34b = data[0] * m22m34 - data[4] * m21m34 + data[12] * m21m32;

		// Return the det of the 4x4
		return data[3] * m12m34 - data[7] * m11m34 + data[11] * m11m34 - data[15] * m11m33; // 25 mul, 14 add
	}
}

template<typename T>
inline Matrix4<T>& Matrix4<T>::inverse(bool* succeeded)
{
	T inv[16];
	inv[0] = data[5] * data[10] * data[15] - data[5] * data[11] * data[14] - data[9] * data[6] * data[15] + data[9] * data[7] * data[14] + data[13] * data[6] * data[11] - data[13] * data[7] * data[10];
	inv[4] = -data[4] * data[10] * data[15] + data[4] * data[11] * data[14] + data[8] * data[6] * data[15] - data[8] * data[7] * data[14] - data[12] * data[6] * data[11] + data[12] * data[7] * data[10];
	inv[8] = data[4] * data[9] * data[15] - data[4] * data[11] * data[13] - data[8] * data[5] * data[15] + data[8] * data[7] * data[13] + data[12] * data[5] * data[11] - data[12] * data[7] * data[9];
	inv[12] = -data[4] * data[9] * data[14] + data[4] * data[10] * data[13] + data[8] * data[5] * data[14] - data[8] * data[6] * data[13] - data[12] * data[5] * data[10] + data[12] * data[6] * data[9];
	inv[1] = -data[1] * data[10] * data[15] + data[1] * data[11] * data[14] + data[9] * data[2] * data[15] - data[9] * data[3] * data[14] - data[13] * data[2] * data[11] + data[13] * data[3] * data[10];
	inv[5] = data[0] * data[10] * data[15] - data[0] * data[11] * data[14] - data[8] * data[2] * data[15] + data[8] * data[3] * data[14] + data[12] * data[2] * data[11] - data[12] * data[3] * data[10];
	inv[9] = -data[0] * data[9] * data[15] + data[0] * data[11] * data[13] + data[8] * data[1] * data[15] - data[8] * data[3] * data[13] - data[12] * data[1] * data[11] + data[12] * data[3] * data[9];
	inv[13] = data[0] * data[9] * data[14] - data[0] * data[10] * data[13] - data[8] * data[1] * data[14] + data[8] * data[2] * data[13] + data[12] * data[1] * data[10] - data[12] * data[2] * data[9];
	inv[2] = data[1] * data[6] * data[15] - data[1] * data[7] * data[14] - data[5] * data[2] * data[15] + data[5] * data[3] * data[14] + data[13] * data[2] * data[7] - data[13] * data[3] * data[6];
	inv[6] = -data[0] * data[6] * data[15] + data[0] * data[7] * data[14] + data[4] * data[2] * data[15] - data[4] * data[3] * data[14] - data[12] * data[2] * data[7] + data[12] * data[3] * data[6];
	inv[10] = data[0] * data[5] * data[15] - data[0] * data[7] * data[13] - data[4] * data[1] * data[15] + data[4] * data[3] * data[13] + data[12] * data[1] * data[7] - data[12] * data[3] * data[5];
	inv[14] = -data[0] * data[5] * data[14] + data[0] * data[6] * data[13] + data[4] * data[1] * data[14] - data[4] * data[2] * data[13] - data[12] * data[1] * data[6] + data[12] * data[2] * data[5];
	inv[3] = -data[1] * data[6] * data[11] + data[1] * data[7] * data[10] + data[5] * data[2] * data[11] - data[5] * data[3] * data[10] - data[9] * data[2] * data[7] + data[9] * data[3] * data[6];
	inv[7] = data[0] * data[6] * data[11] - data[0] * data[7] * data[10] - data[4] * data[2] * data[11] + data[4] * data[3] * data[10] + data[8] * data[2] * data[7] - data[8] * data[3] * data[6];
	inv[11] = -data[0] * data[5] * data[11] + data[0] * data[7] * data[9] + data[4] * data[1] * data[11] - data[4] * data[3] * data[9] - data[8] * data[1] * data[7] + data[8] * data[3] * data[5];
	inv[15] = data[0] * data[5] * data[10] - data[0] * data[6] * data[9] - data[4] * data[1] * data[10] + data[4] * data[2] * data[9] + data[8] * data[1] * data[6] - data[8] * data[2] * data[5];

	const T det = data[0] * inv[0] + data[1] * inv[4] + data[2] * inv[8] + data[3] * inv[12];
	if (Math::Equals(det, T(0)))
	{
		if (succeeded != nullptr)
		{
			*succeeded = false;
		}
		return *this;
	}

	const T invDet = 1 / det;
	for (int i = 0; i < 16; i++)
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
inline Matrix4<T> Matrix4<T>::inversed(bool* succeeded) const
{
	T inv[16];
	inv[0] = data[5] * data[10] * data[15] - data[5] * data[11] * data[14] - data[9] * data[6] * data[15] + data[9] * data[7] * data[14] + data[13] * data[6] * data[11] - data[13] * data[7] * data[10];
	inv[4] = -data[4] * data[10] * data[15] + data[4] * data[11] * data[14] + data[8] * data[6] * data[15] - data[8] * data[7] * data[14] - data[12] * data[6] * data[11] + data[12] * data[7] * data[10];
	inv[8] = data[4] * data[9] * data[15] - data[4] * data[11] * data[13] - data[8] * data[5] * data[15] + data[8] * data[7] * data[13] + data[12] * data[5] * data[11] - data[12] * data[7] * data[9];
	inv[12] = -data[4] * data[9] * data[14] + data[4] * data[10] * data[13] + data[8] * data[5] * data[14] - data[8] * data[6] * data[13] - data[12] * data[5] * data[10] + data[12] * data[6] * data[9];
	inv[1] = -data[1] * data[10] * data[15] + data[1] * data[11] * data[14] + data[9] * data[2] * data[15] - data[9] * data[3] * data[14] - data[13] * data[2] * data[11] + data[13] * data[3] * data[10];
	inv[5] = data[0] * data[10] * data[15] - data[0] * data[11] * data[14] - data[8] * data[2] * data[15] + data[8] * data[3] * data[14] + data[12] * data[2] * data[11] - data[12] * data[3] * data[10];
	inv[9] = -data[0] * data[9] * data[15] + data[0] * data[11] * data[13] + data[8] * data[1] * data[15] - data[8] * data[3] * data[13] - data[12] * data[1] * data[11] + data[12] * data[3] * data[9];
	inv[13] = data[0] * data[9] * data[14] - data[0] * data[10] * data[13] - data[8] * data[1] * data[14] + data[8] * data[2] * data[13] + data[12] * data[1] * data[10] - data[12] * data[2] * data[9];
	inv[2] = data[1] * data[6] * data[15] - data[1] * data[7] * data[14] - data[5] * data[2] * data[15] + data[5] * data[3] * data[14] + data[13] * data[2] * data[7] - data[13] * data[3] * data[6];
	inv[6] = -data[0] * data[6] * data[15] + data[0] * data[7] * data[14] + data[4] * data[2] * data[15] - data[4] * data[3] * data[14] - data[12] * data[2] * data[7] + data[12] * data[3] * data[6];
	inv[10] = data[0] * data[5] * data[15] - data[0] * data[7] * data[13] - data[4] * data[1] * data[15] + data[4] * data[3] * data[13] + data[12] * data[1] * data[7] - data[12] * data[3] * data[5];
	inv[14] = -data[0] * data[5] * data[14] + data[0] * data[6] * data[13] + data[4] * data[1] * data[14] - data[4] * data[2] * data[13] - data[12] * data[1] * data[6] + data[12] * data[2] * data[5];
	inv[3] = -data[1] * data[6] * data[11] + data[1] * data[7] * data[10] + data[5] * data[2] * data[11] - data[5] * data[3] * data[10] - data[9] * data[2] * data[7] + data[9] * data[3] * data[6];
	inv[7] = data[0] * data[6] * data[11] - data[0] * data[7] * data[10] - data[4] * data[2] * data[11] + data[4] * data[3] * data[10] + data[8] * data[2] * data[7] - data[8] * data[3] * data[6];
	inv[11] = -data[0] * data[5] * data[11] + data[0] * data[7] * data[9] + data[4] * data[1] * data[11] - data[4] * data[3] * data[9] - data[8] * data[1] * data[7] + data[8] * data[3] * data[5];
	inv[15] = data[0] * data[5] * data[10] - data[0] * data[6] * data[9] - data[4] * data[1] * data[10] + data[4] * data[2] * data[9] + data[8] * data[1] * data[6] - data[8] * data[2] * data[5];

	const T det = data[0] * inv[0] + data[1] * inv[4] + data[2] * inv[8] + data[3] * inv[12];
	if (Math::Equals(det, T(0)))
	{
		if (succeeded != nullptr)
		{
			*succeeded = false;
		}
		return identity;
	}

	const T invDet = 1 / det;
	for (int i = 0; i < 16; i++)
	{
		inv[i] *= invDet;
	}

	if (succeeded != nullptr)
	{
		*succeeded = true;
	}
	return Matrix4<T>(inv);
}

template<typename T>
inline Matrix4<T>& Matrix4<T>::transpose()
{
	std::swap(data[1], data[4]);
	std::swap(data[2], data[8]);
	std::swap(data[3], data[12]);
	std::swap(data[6], data[9]);
	std::swap(data[7], data[13]);
	std::swap(data[11], data[14]);
	return *this;
}

template<typename T>
inline Matrix4<T> Matrix4<T>::transposed() const
{
	return Matrix4<T>(data[0], data[4], data[8], data[12], data[1], data[5], data[9], data[13], data[2], data[6], data[10], data[14], data[3], data[7], data[11], data[15]);
}

template<typename T>
inline Matrix4<T>& Matrix4<T>::setRotation(const Quaternion<T>& rotation)
{
	return fromMatrix3(rotation.toMatrix3());
}

template<typename T>
inline Matrix4<T>& Matrix4<T>::setRotation(const Matrix3<T>& rotation)
{
	return fromMatrix3(rotation);
}

template<typename T>
inline Matrix4<T>& Matrix4<T>::setScale(const Vector3<T>& scale)
{
	data[0] = scale.x;
	data[1] = 0;
	data[2] = 0;
	data[4] = 0;
	data[5] = scale.y;
	data[6] = 0;
	data[8] = 0;
	data[9] = 0;
	data[10] = scale.z;
}

template<typename T>
inline Matrix4<T>& Matrix4<T>::setScale(const T& sx, const T& sy, const T& sz)
{
	data[0] = sx;
	data[1] = 0;
	data[2] = 0;
	data[4] = 0;
	data[5] = sy;
	data[6] = 0;
	data[8] = 0;
	data[9] = 0;
	data[10] = sz;
}

template<typename T>
inline Matrix4<T>& Matrix4<T>::setScale(const T& s)
{
	data[0] = s;
	data[1] = 0;
	data[2] = 0;
	data[4] = 0;
	data[5] = s;
	data[6] = 0;
	data[8] = 0;
	data[9] = 0;
	data[10] = s;
}

template<typename T>
inline Matrix4<T>& Matrix4<T>::setTranslation(const Vector3<T>& translation)
{
	data[12] = translation.x;
	data[13] = translation.y;
	data[14] = translation.z;
	return *this;
}

template<typename T>
inline Matrix4<T>& Matrix4<T>::setTranslation(const T& tx, const T& ty, const T& tz)
{
	data[12] = tx;
	data[13] = ty;
	data[14] = tz;
	return *this;
}

template<typename T>
inline Matrix4<T>& Matrix4<T>::applyRotation(const Quaternion<T>& rotation)
{
	// TODO : Improve ?
	operator*=(Matrix4<T>::rotation(rotation));
	return *this;
}

template<typename T>
inline Matrix4<T>& Matrix4<T>::applyRotation(const Matrix3<T>& rotation)
{
	// TODO : Improve ?
	operator*=(Matrix4<T>::rotation(rotation));
	return *this;
}

template<typename T>
inline Matrix4<T>& Matrix4<T>::applyScale(const Vector3<T>& scale)
{
	// TODO : Improve ? 
	operator*=(Matrix4<T>::scale(scale));
	return *this;
}

template<typename T>
inline Matrix4<T>& Matrix4<T>::applyScale(const T& sx, const T& sy, const T& sz)
{
	// TODO : Improve ? 
	operator*=(Matrix4<T>::scale(sx, sy, sz));
	return *this;
}

template<typename T>
inline Matrix4<T>& Matrix4<T>::applyScale(const T& s)
{
	// TODO : Improve ? 
	operator*=(Matrix4<T>::scale(s));
	return *this;
}

template<typename T>
inline Matrix4<T>& Matrix4<T>::applyTranslation(const Vector3<T>& translation)
{
	data[12] += translation.x;
	data[13] += translation.y;
	data[14] += translation.z;
	return *this;
}

template<typename T>
inline Matrix4<T>& Matrix4<T>::applyTranslation(const T& tx, const T& ty, const T& tz)
{
	data[12] += tx;
	data[13] += ty;
	data[14] += tz;
	return *this;
}

template<typename T>
inline Matrix4<T> Matrix4<T>::rotated(const Quaternion<T>& rotation)
{
	// TODO : Improve ? 
	return Matrix4<T>(*this).applyRotation(rotation);
}

template<typename T>
inline Matrix4<T> Matrix4<T>::rotated(const Matrix3<T>& rotation)
{
	// TODO : Improve ? 
	return Matrix4<T>(*this).applyRotation(rotation);
}

template<typename T>
inline Matrix4<T> Matrix4<T>::scaled(const Vector3<T>& scale)
{
	// TODO : Improve ? 
	return Matrix4<T>(*this).applyScale(scale);
}

template<typename T>
inline Matrix4<T> Matrix4<T>::scaled(const T& sx, const T& sy, const T& sz)
{
	// TODO : Improve ?
	return Matrix4<T>(*this).applyScale(sx, sy, sz);
}

template<typename T>
inline Matrix4<T> Matrix4<T>::scaled(const T& s)
{
	// TODO : Improve ?
	return Matrix4<T>(*this).applyScale(s);
}

template<typename T>
inline Matrix4<T> Matrix4<T>::translated(const Vector3<T>& translation)
{
	// TODO : Improve ? 
	return Matrix4<T>(*this).applyTranslation(translation);
}

template<typename T>
inline Matrix4<T> Matrix4<T>::translated(const T& tx, const T& ty, const T& tz)
{
	// TODO : Improve ?
	return Matrix4<T>(*this).applyTranslation(tx, ty, tz);
}

template<typename T>
inline Matrix4<T> Matrix4<T>::transformed(const Vector3<T>& translation, const Quaternion<T>& rotation)
{
	// TODO : Improve ? 
	return Matrix4<T>(*this).applyTransform(translation, rotation);
}

template<typename T>
inline Matrix4<T> Matrix4<T>::transformed(const Vector3<T>& translation, const Quaternion<T>& rotation, const Vector3<T>& scale)
{
	// TODO : Improve ? 
	return Matrix4<T>(*this).applyTransform(translation, rotation, scale);
}

template<typename T>
inline Matrix4<T>& Matrix4<T>::makeRotation(const Quaternion<T>& rotation)
{
	// TODO : Improve ? 
	return set(Matrix4<T>::rotation(rotation));
}

template<typename T>
inline Matrix4<T>& Matrix4<T>::makeRotation(const Matrix3<T>& rotation)
{
	// TODO : Improve ? 
	return set(Matrix4<T>::rotation(rotation));
}

template<typename T>
inline Matrix4<T>& Matrix4<T>::makeScale(const Vector3<T>& scale)
{
	// TODO : Improve ? 
	return set(Matrix4<T>::scale(scale));
}

template<typename T>
inline Matrix4<T>& Matrix4<T>::makeTranslation(const Vector3<T>& translation)
{
	// TODO : Improve ? 
	return set(Matrix4<T>::translation(translation));
}

template<typename T>
inline Matrix4<T>& Matrix4<T>::makeTransform(const Vector3<T>& translation, const Quaternion<T>& rotation)
{
	// TODO : Improve ? 
	return set(Matrix4<T>::transform(translation, rotation));
}

template<typename T>
inline Matrix4<T>& Matrix4<T>::makeTransform(const Vector3<T>& translation, const Quaternion<T>& rotation, const Vector3<T>& scale)
{
	// TODO : Improve ? 
	return set(Matrix4<T>::transform(translation, rotation, scale));
}

template<typename T>
inline Matrix4<T>& Matrix4<T>::makeViewMatrix(const Vector3<T>& translation, const Quaternion<T>& rotation)
{
	// TODO : Improve ? 
	return set(Matrix4<T>::viewMatrix(translation, rotation));
}

template<typename T>
inline Matrix4<T>& Matrix4<T>::makeLookAt(const Vector3<T>& eye, const Vector3<T>& target, const Vector3<T>& up)
{
	// TODO : Improve ? 
	return set(Matrix4<T>::lookAt(eye, target, up));
}

template<typename T>
inline Matrix4<T>& Matrix4<T>::makeOrtho(const T& left, const T& right, const T& top, const T& bottom, const T& zNear, const T& zFar)
{
	// TODO : Improve ? 
	return set(Matrix4<T>::ortho(left, right, top, bottom, zNear, zFar));
}

template<typename T>
inline Matrix4<T>& Matrix4<T>::makePerspective(const T& fov, const T& ratio, const T& zNear, const T& zFar)
{
	// TODO : Improve ? 
	return set(Matrix4<T>::perspective(fov, ratio, zNear, zFar));
}

template<typename T>
inline Matrix4<T>& Matrix4<T>::makeZero()
{
	return set(Matrix4<T>::zero);
}

template<typename T>
inline Matrix4<T>& Matrix4<T>::makeIdentity()
{
	return set(Matrix4<T>::identity);
}

template<typename T>
inline Matrix4<T> Matrix4<T>::rotation(const Quaternion<T>& rotation)
{
	return rotation.toMatrix4();
}

template<typename T>
inline Matrix4<T> Matrix4<T>::rotation(const Matrix3<T>& rotation)
{
	return Matrix4<T>(rotation);
}

template<typename T>
inline Matrix4<T> Matrix4<T>::scale(const Vector3<T>& scale)
{
	return Matrix4<T>(scale.x, T(0), T(0), T(0), T(0), scale.y, T(0), T(0), T(0), T(0), scale.z, T(0), T(0), T(0), T(0), T(1));
}

template<typename T>
inline Matrix4<T> Matrix4<T>::scale(const T& sx, const T& sy, const T& sz)
{
	return Matrix4<T>(sx, T(0), T(0), T(0), T(0), sy, T(0), T(0), T(0), T(0), sz, T(0), T(0), T(0), T(0), T(1));
}

template<typename T>
inline Matrix4<T> Matrix4<T>::scale(const T& s)
{
	return Matrix4<T>(s, T(0), T(0), T(0), T(0), s, T(0), T(0), T(0), T(0), s, T(0), T(0), T(0), T(0), T(1));
}

template<typename T>
inline Matrix4<T> Matrix4<T>::translation(const Vector3<T>& translation)
{
	return Matrix4<T>(T(1), T(0), T(0), T(0), T(0), T(1), T(0), T(0), T(0), T(0), T(1), T(0), translation.x, translation.y, translation.z, T(1));
}

template<typename T>
inline Matrix4<T> Matrix4<T>::translation(const T & tx, const T & ty, const T & tz)
{
	return Matrix4<T>(T(1), T(0), T(0), T(0), T(0), T(1), T(0), T(0), T(0), T(0), T(1), T(0), tx, ty, tz, T(1));
}

template<typename T>
inline Matrix4<T> Matrix4<T>::transform(const Vector3<T>& translation, const Quaternion<T>& rotation)
{
	enAssert(false); // Wrong, do not use
	Matrix4<T> m;
	m.setRotation(rotation);
	m.setTranslation(translation);
	m.data[3] = 0;
	m.data[7] = 0;
	m.data[11] = 0;
	m.data[15] = 1;
	return m;
}

template<typename T>
inline Matrix4<T> Matrix4<T>::transform(const Vector3<T>& translation, const Quaternion<T>& rotation, const Vector3<T>& scale)
{
	enAssert(false); // Wrong, do not use
	Matrix4<T> m;
	m.setRotation(rotation);
	m.setTranslation(translation);
	m.applyScale(scale);
	m.data[3] = 0;
	m.data[7] = 0;
	m.data[11] = 0;
	m.data[15] = 1;
	return m;
}

template<typename T>
inline Matrix4<T> Matrix4<T>::viewMatrix(const Vector3<T>& translation, const Quaternion<T>& rotation)
{
	Quaternion<T> invRot = rotation.conjugated();
	return transform(-(invRot * translation), invRot);
}

template<typename T>
inline Matrix4<T> Matrix4<T>::lookAt(const Vector3<T>& eye, const Vector3<T>& target, const Vector3<T>& up)
{
	const Vector3f f((target - eye).Normalized());
	const Vector3f s(f.CrossProduct(up).Normalized());
	const Vector3f u(s.CrossProduct(f));
	return Matrix4<T>(s.x, u.x, -f.x, 0, s.y, u.y, -f.y, 0, s.z, u.z, -f.z, 0, -s.DotProduct(eye), -u.DotProduct(eye), f.DotProduct(eye), 1);
}

template<typename T>
inline Matrix4<T> Matrix4<T>::ortho(const T& left, const T& right, const T& top, const T& bottom, const T& zNear, const T& zFar)
{
	const T width = right - left;
	const T height = top - bottom;
	const T zDist = zFar - zNear;
	return Matrix4<T>(2 / width, 0, 0, 0, 0, 2 / height, 0, 0, 0, 0, 2 / zDist, 0, -(right + left) / width, -(top + bottom) / height, -(zFar + zNear) / zDist, 1);
}

template<typename T>
inline Matrix4<T> Matrix4<T>::perspective(const T& fov, const T& ratio, const T& zNear, const T& zFar)
{
	enAssert(ratio != 0.0f);
	const T y = 1 / Math::Tan(fov * T(0.5));
	const T x = y / ratio;
	const T zDist = zFar - zNear;
	return Matrix4<T>(x, 0, 0, 0, 0, y, 0, 0, 0, 0, -(zFar + zNear) / zDist, -1, 0, 0, (-2 * zFar * zNear) / zDist, 0);
}

template <typename T> const Matrix4<T> Matrix4<T>::zero(0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
template <typename T> const Matrix4<T> Matrix4<T>::identity(1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1);

typedef Matrix4<F32> Matrix4f;

typedef Matrix4f mat4; // GLSL-like

} // namespace en

ENLIVE_DEFINE_TYPE_INFO_TEMPLATE(en::Matrix4)
