#pragma once

#include <Enlivengine/Config.hpp>

#ifdef ENLIVE_MODULE_MATH

#include <Enlivengine/Math/Vector4.hpp>
#include <Enlivengine/Math/Matrix3.hpp>
#include <Enlivengine/Math/Quaternion.hpp>

// TODO : Constexpr memset, memcpy, swap, LookAt

namespace en
{

// 
// Matrix layout :    
// 
// 0  1  2  3      a11 a12 a13 a14
// 4  5  6  7   =  a21 a22 a23 a24
// 8  9  10 11     a31 a32 a33 a34
// 12 13 14 15     a41 a42 a43 a44
//

template <typename T>
class Matrix4
{
public:
	static constexpr I32 Rows{ 4 };
	static constexpr I32 Columns{ 4 };
	static constexpr I32 Elements{ Rows * Columns };

	constexpr Matrix4() : data{ T(1), T(0), T(0), T(0), T(0), T(1), T(0), T(0), T(0), T(0), T(1), T(0), T(0), T(0), T(0), T(1) } {}
	constexpr Matrix4(const Matrix4<T>& m) : data{ m.data[0], m.data[1], m.data[2], m.data[3], m.data[4], m.data[5], m.data[6], m.data[7], m.data[8], m.data[9], m.data[10], m.data[11], m.data[12], m.data[13], m.data[14], m.data[15] } {}
	template <typename U>
	constexpr Matrix4(const Matrix4<U>& m) : data{ static_cast<T>(m.data[0]), static_cast<T>(m.data[1]), static_cast<T>(m.data[2]), static_cast<T>(m.data[3]), static_cast<T>(m.data[4]), static_cast<T>(m.data[5]), static_cast<T>(m.data[6]), static_cast<T>(m.data[7]), static_cast<T>(m.data[8]), static_cast<T>(m.data[9]), static_cast<T>(m.data[10]), static_cast<T>(m.data[11]), static_cast<T>(m.data[12]), static_cast<T>(m.data[13]), static_cast<T>(m.data[14]), static_cast<T>(m.data[15]) } {}
	constexpr Matrix4(const T& a11, const T& a12, const T& a13, const T& a14, const T& a21, const T& a22, const T& a23, const T& a24, const T& a31, const T& a32, const T& a33, const T& a34, const T& a41, const T& a42, const T& a43, const T& a44) : data{ a11, a12, a13, a14, a21, a22, a23, a24, a31, a32, a33, a34, a41, a42, a43, a44 } {}
	~Matrix4() = default;

	constexpr Matrix4<T>& Set(const Matrix4<T> & m)
	{
		data[0] = m.data[0];
		data[1] = m.data[1];
		data[2] = m.data[2];
		data[3] = m.data[3];
		data[4] = m.data[4];
		data[5] = m.data[5];
		data[6] = m.data[6];
		data[7] = m.data[7];
		data[8] = m.data[8];
		data[9] = m.data[9];
		data[10] = m.data[10];
		data[11] = m.data[11];
		data[12] = m.data[12];
		data[13] = m.data[13];
		data[14] = m.data[14];
		data[15] = m.data[15];
		return *this;
	}
	template <typename U>
	constexpr Matrix4<T>& Set(const Matrix4<U> & m)
	{
		data[0] = static_cast<T>(m.data[0]);
		data[1] = static_cast<T>(m.data[1]);
		data[2] = static_cast<T>(m.data[2]);
		data[3] = static_cast<T>(m.data[3]);
		data[4] = static_cast<T>(m.data[4]);
		data[5] = static_cast<T>(m.data[5]);
		data[6] = static_cast<T>(m.data[6]);
		data[7] = static_cast<T>(m.data[7]);
		data[8] = static_cast<T>(m.data[8]);
		data[9] = static_cast<T>(m.data[9]);
		data[10] = static_cast<T>(m.data[10]);
		data[11] = static_cast<T>(m.data[11]);
		data[12] = static_cast<T>(m.data[12]);
		data[13] = static_cast<T>(m.data[13]);
		data[14] = static_cast<T>(m.data[14]);
		data[15] = static_cast<T>(m.data[15]);
		return *this;
	}
	constexpr Matrix4<T>& Set(const T * a)
	{
		data[0] = a[0];
		data[1] = a[1];
		data[2] = a[2];
		data[3] = a[3];
		data[4] = a[4];
		data[5] = a[5];
		data[6] = a[6];
		data[7] = a[7];
		data[8] = a[8];
		data[9] = a[9];
		data[10] = a[10];
		data[11] = a[11];
		data[12] = a[12];
		data[13] = a[13];
		data[14] = a[14];
		data[15] = a[15];
		return *this;
	}
	constexpr Matrix4<T>& Set(const T & a11, const T & a21, const T & a31, const T & a41, const T & a12, const T & a22, const T & a32, const T & a42, const T & a13, const T & a23, const T & a33, const T & a43, const T & a14, const T & a24, const T & a34, const T & a44)
	{
		data[0] = a11;
		data[1] = a12;
		data[2] = a13;
		data[3] = a14;
		data[4] = a21;
		data[5] = a22;
		data[6] = a23;
		data[7] = a24;
		data[8] = a31;
		data[9] = a32;
		data[10] = a33;
		data[11] = a34;
		data[12] = a41;
		data[13] = a42;
		data[14] = a43;
		data[15] = a44;
		return *this;
	}

	constexpr T& operator[](U32 i) { return data[i]; }
	constexpr const T& operator[](U32 i) const { return data[i]; }
	constexpr T& operator()(U32 row, U32 column) { return data[column + Columns * row]; }
	constexpr const T& operator()(U32 row, U32 column) const { return data[column + Columns * row]; }

	constexpr Vector4<T> GetColumn(U32 columnIndex) const { return Vector4<T>(data[columnIndex], data[columnIndex + Columns], data[columnIndex + Columns * 2], data[columnIndex + Columns * 3]); }
	constexpr Vector4<T> GetRow(U32 rowIndex) const { const U32 r = Columns * rowIndex; return Vector4<T>(data[r], data[r + 1], data[r + 2], data[r + 3]); }
	constexpr Matrix4<T>& SetColumn(U32 columnIndex, const Vector4<T> & column)
	{
		data[columnIndex] = column.x;
		data[columnIndex + Columns * 1] = column.y;
		data[columnIndex + Columns * 2] = column.z;
		data[columnIndex + Columns * 3] = column.w;
		return *this;
	}
	constexpr Matrix4<T>& SetRow(U32 rowIndex, const Vector4<T> & row)
	{
		const U32 r = Columns * rowIndex;
		data[r] = row.x;
		data[r + 1] = row.y;
		data[r + 2] = row.z;
		data[r + 3] = row.w;
		return *this;
	}

	constexpr Matrix4<T>& operator=(const Matrix4<T> & m) { return Set(m); }
	constexpr const Matrix4<T>& operator+() const { return *this; }
	constexpr Matrix4<T> operator-() const { return Matrix4<T>(-data[0], -data[1], -data[2], -data[3], -data[4], -data[5], -data[6], -data[7], -data[8], -data[9], -data[10], -data[11], -data[12], -data[13], -data[14], -data[15]); }

	constexpr Matrix4<T> operator+(const Matrix4<T> & m) const { return Matrix4<T>(data[0] + m.data[0], data[1] + m.data[1], data[2] + m.data[2], data[3] + m.data[3], data[4] + m.data[4], data[5] + m.data[5], data[6] + m.data[6], data[7] + m.data[7], data[8] + m.data[8], data[9] + m.data[9], data[10] + m.data[10], data[11] + m.data[11], data[12] + m.data[12], data[13] + m.data[13], data[14] + m.data[14], data[15] + m.data[15]); }
	constexpr Matrix4<T> operator-(const Matrix4<T> & m) const { return Matrix4<T>(data[0] - m.data[0], data[1] - m.data[1], data[2] - m.data[2], data[3] - m.data[3], data[4] - m.data[4], data[5] - m.data[5], data[6] - m.data[6], data[7] - m.data[7], data[8] - m.data[8], data[9] - m.data[9], data[10] - m.data[10], data[11] - m.data[11], data[12] - m.data[12], data[13] - m.data[13], data[14] - m.data[14], data[15] - m.data[15]); }
	constexpr Matrix4<T> operator*(const Matrix4<T> & m) const
	{
		Matrix4<T> out;
		const Vector4<T> r0(GetRow(0));
		const Vector4<T> r1(GetRow(1));
		const Vector4<T> r2(GetRow(2));
		const Vector4<T> r3(GetRow(3));
		const Vector4<T> c0(m.GetColumn(0));
		const Vector4<T> c1(m.GetColumn(1));
		const Vector4<T> c2(m.GetColumn(2));
		const Vector4<T> c3(m.GetColumn(3));
		out.data[0] = c0.DotProduct(r0);
		out.data[1] = c1.DotProduct(r0);
		out.data[2] = c2.DotProduct(r0);
		out.data[3] = c3.DotProduct(r0);
		out.data[4] = c0.DotProduct(r1);
		out.data[5] = c1.DotProduct(r1);
		out.data[6] = c2.DotProduct(r1);
		out.data[7] = c3.DotProduct(r1);
		out.data[8] = c0.DotProduct(r2);
		out.data[9] = c1.DotProduct(r2);
		out.data[10] = c2.DotProduct(r2);
		out.data[11] = c3.DotProduct(r2);
		out.data[12] = c0.DotProduct(r3);
		out.data[13] = c1.DotProduct(r3);
		out.data[14] = c2.DotProduct(r3);
		out.data[15] = c3.DotProduct(r3);
		return out;
	}
	constexpr Matrix4<T>& operator+=(const Matrix4<T> & m) { data[0] += m.data[0]; data[1] += m.data[1]; data[2] += m.data[2]; data[3] += m.data[3]; data[4] += m.data[4]; data[5] += m.data[5]; data[6] += m.data[6]; data[7] += m.data[7]; data[8] += m.data[8]; data[9] += m.data[9]; data[10] += m.data[10]; data[11] += m.data[11]; data[12] += m.data[12]; data[13] += m.data[13]; data[14] += m.data[14]; data[15] += m.data[15]; return *this; }
	constexpr Matrix4<T>& operator-=(const Matrix4<T> & m) { data[0] -= m.data[0]; data[1] -= m.data[1]; data[2] -= m.data[2]; data[3] -= m.data[3]; data[4] -= m.data[4]; data[5] -= m.data[5]; data[6] -= m.data[6]; data[7] -= m.data[7]; data[8] -= m.data[8]; data[9] -= m.data[9]; data[10] -= m.data[10]; data[11] -= m.data[11]; data[12] -= m.data[12]; data[13] -= m.data[13]; data[14] -= m.data[14]; data[15] -= m.data[15]; return *this; }
	constexpr Matrix4<T>& operator*=(const Matrix4<T> & m)
	{
		const Vector4<T> r0(GetRow(0));
		const Vector4<T> r1(GetRow(1));
		const Vector4<T> r2(GetRow(2));
		const Vector4<T> r3(GetRow(3));
		const Vector4<T> c0(m.GetColumn(0));
		const Vector4<T> c1(m.GetColumn(1));
		const Vector4<T> c2(m.GetColumn(2));
		const Vector4<T> c3(m.GetColumn(3));
		data[0] = c0.DotProduct(r0);
		data[1] = c1.DotProduct(r0);
		data[2] = c2.DotProduct(r0);
		data[3] = c3.DotProduct(r0);
		data[4] = c0.DotProduct(r1);
		data[5] = c1.DotProduct(r1);
		data[6] = c2.DotProduct(r1);
		data[7] = c3.DotProduct(r1);
		data[8] = c0.DotProduct(r2);
		data[9] = c1.DotProduct(r2);
		data[10] = c2.DotProduct(r2);
		data[11] = c3.DotProduct(r2);
		data[12] = c0.DotProduct(r3);
		data[13] = c1.DotProduct(r3);
		data[14] = c2.DotProduct(r3);
		data[15] = c3.DotProduct(r3);
		return *this;
	}

	constexpr Matrix4<T> operator*(const T & s) const { return Matrix4<T>(data[0] * s, data[1] * s, data[2] * s, data[3] * s, data[4] * s, data[5] * s, data[6] * s, data[7] * s, data[8] * s, data[9] * s, data[10] * s, data[11] * s, data[12] * s, data[13] * s, data[14] * s, data[15] * s); }
	constexpr Matrix4<T> operator/(const T & s) const { const T inv = T(1) / s; return Matrix4<T>(data[0] * inv, data[1] * inv, data[2] * inv, data[3] * inv, data[4] * inv, data[5] * inv, data[6] * inv, data[7] * inv, data[8] * inv, data[9] * inv, data[10] * inv, data[11] * inv, data[12] * inv, data[13] * inv, data[14] * inv, data[15] * inv); }
	constexpr Matrix4<T>& operator*=(const T & s) { data[0] *= s; data[1] *= s; data[2] *= s; data[3] *= s; data[4] *= s; data[5] *= s; data[6] *= s; data[7] *= s; data[8] *= s; data[9] *= s; data[10] *= s; data[11] *= s; data[12] *= s; data[13] *= s; data[14] *= s; data[15] *= s; return *this; }
	constexpr Matrix4<T>& operator/=(const T & s) { const T inv = T(1) / s; data[0] *= inv; data[1] *= inv; data[2] *= inv; data[3] *= inv; data[4] *= inv; data[5] *= inv; data[6] *= inv; data[7] *= inv; data[8] *= inv; data[9] *= inv; data[10] *= inv; data[11] *= inv; data[12] *= inv; data[13] *= inv; data[14] *= inv; data[15] *= inv; return *this; }

	constexpr bool operator==(const Matrix4<T> & m) const
	{
		return Math::Equals(data[0], m.data[0])
			&& Math::Equals(data[1], m.data[1])
			&& Math::Equals(data[2], m.data[2])
			&& Math::Equals(data[3], m.data[3])
			&& Math::Equals(data[4], m.data[4])
			&& Math::Equals(data[5], m.data[5])
			&& Math::Equals(data[6], m.data[6])
			&& Math::Equals(data[7], m.data[7])
			&& Math::Equals(data[8], m.data[8])
			&& Math::Equals(data[9], m.data[9])
			&& Math::Equals(data[10], m.data[10])
			&& Math::Equals(data[11], m.data[11])
			&& Math::Equals(data[12], m.data[12])
			&& Math::Equals(data[13], m.data[13])
			&& Math::Equals(data[14], m.data[14])
			&& Math::Equals(data[15], m.data[15]);
	}
	constexpr bool operator!=(const Matrix4<T> & m) const { return !operator==(m); }
	constexpr bool IsIdentity() const { return operator==(Identity()); }

	constexpr bool IsAffine() const
	{
		return Math::Equals(T(0), data[3])
			&& Math::Equals(T(0), data[7])
			&& Math::Equals(T(0), data[11])
			&& Math::Equals(T(1), data[15]);
	}

	constexpr T GetTrace() const { return data[0] + data[5] + data[10] + data[15]; }
	constexpr T GetDeterminant() const
	{
		// We will use the last row of the matrix
		// Because if the matrix is affine it will be faster

		// Compute det2x2 on the 2 middle columns
		const T a = data[5] * data[10] - data[6] * data[9];
		const T b = data[1] * data[10] - data[2] * data[9];
		const T c = data[1] * data[6] - data[2] * data[5];

		// Compute det3x3 using first column
		const T det15 = data[0] * a - data[4] * b + data[8] * c;

		// Compute det2x2
		if (IsAffine())
		{
			// If affine, the last column is full of zero except 15 which is one
			// Return the det of the 3x3
			return det15; // 9 mul, 5 add
		}
		else
		{
			// Compute det2x2, each det2x2 is used 2 times for the det3
			const T d = data[1] * data[14] - data[2] * data[13];
			const T e = data[5] * data[14] - data[6] * data[13];
			const T f = data[9] * data[14] - data[10] * data[13];

			// Compute det3x3 using the first column
			const T det3 = data[4] * f - data[8] * e + data[12] * a;
			const T det7 = data[0] * f - data[8] * d + data[12] * b;
			const T det11 = data[0] * e - data[4] * d + data[12] * c;

			// Return the det of the 4x4
			return -data[3] * det3 + data[7] * det7 - data[11] * det11 + data[15] * det15; // 25 mul, 14 add
		}
	}

	constexpr Matrix4<T>& Inverse(bool* succeeded = nullptr)
	{
		// TODO : This could be improved if the matrix is affine
		T inv[16];
		inv[0] = data[5] * data[10] * data[15] - data[5] * data[11] * data[14] - data[9] * data[6] * data[15] + data[9] * data[7] * data[14] + data[13] * data[6] * data[11] - data[13] * data[7] * data[10];
		inv[1] = -data[1] * data[10] * data[15] + data[1] * data[11] * data[14] + data[9] * data[2] * data[15] - data[9] * data[3] * data[14] - data[13] * data[2] * data[11] + data[13] * data[3] * data[10];
		inv[2] = data[1] * data[6] * data[15] - data[1] * data[7] * data[14] - data[5] * data[2] * data[15] + data[5] * data[3] * data[14] + data[13] * data[2] * data[7] - data[13] * data[3] * data[6];
		inv[3] = -data[1] * data[6] * data[11] + data[1] * data[7] * data[10] + data[5] * data[2] * data[11] - data[5] * data[3] * data[10] - data[9] * data[2] * data[7] + data[9] * data[3] * data[6];
		inv[4] = -data[4] * data[10] * data[15] + data[4] * data[11] * data[14] + data[8] * data[6] * data[15] - data[8] * data[7] * data[14] - data[12] * data[6] * data[11] + data[12] * data[7] * data[10];
		inv[5] = data[0] * data[10] * data[15] - data[0] * data[11] * data[14] - data[8] * data[2] * data[15] + data[8] * data[3] * data[14] + data[12] * data[2] * data[11] - data[12] * data[3] * data[10];
		inv[6] = -data[0] * data[6] * data[15] + data[0] * data[7] * data[14] + data[4] * data[2] * data[15] - data[4] * data[3] * data[14] - data[12] * data[2] * data[7] + data[12] * data[3] * data[6];
		inv[7] = data[0] * data[6] * data[11] - data[0] * data[7] * data[10] - data[4] * data[2] * data[11] + data[4] * data[3] * data[10] + data[8] * data[2] * data[7] - data[8] * data[3] * data[6];
		inv[8] = data[4] * data[9] * data[15] - data[4] * data[11] * data[13] - data[8] * data[5] * data[15] + data[8] * data[7] * data[13] + data[12] * data[5] * data[11] - data[12] * data[7] * data[9];
		inv[9] = -data[0] * data[9] * data[15] + data[0] * data[11] * data[13] + data[8] * data[1] * data[15] - data[8] * data[3] * data[13] - data[12] * data[1] * data[11] + data[12] * data[3] * data[9];
		inv[10] = data[0] * data[5] * data[15] - data[0] * data[7] * data[13] - data[4] * data[1] * data[15] + data[4] * data[3] * data[13] + data[12] * data[1] * data[7] - data[12] * data[3] * data[5];
		inv[11] = -data[0] * data[5] * data[11] + data[0] * data[7] * data[9] + data[4] * data[1] * data[11] - data[4] * data[3] * data[9] - data[8] * data[1] * data[7] + data[8] * data[3] * data[5];
		inv[12] = -data[4] * data[9] * data[14] + data[4] * data[10] * data[13] + data[8] * data[5] * data[14] - data[8] * data[6] * data[13] - data[12] * data[5] * data[10] + data[12] * data[6] * data[9];
		inv[13] = data[0] * data[9] * data[14] - data[0] * data[10] * data[13] - data[8] * data[1] * data[14] + data[8] * data[2] * data[13] + data[12] * data[1] * data[10] - data[12] * data[2] * data[9];
		inv[14] = -data[0] * data[5] * data[14] + data[0] * data[6] * data[13] + data[4] * data[1] * data[14] - data[4] * data[2] * data[13] - data[12] * data[1] * data[6] + data[12] * data[2] * data[5];
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
		return Set(inv);
	}
	constexpr Matrix4<T> Inversed(bool* succeeded = nullptr) const
	{
		// TODO : This could be improved if the matrix is affine
		T inv[16];
		inv[0] = data[5] * data[10] * data[15] - data[5] * data[11] * data[14] - data[9] * data[6] * data[15] + data[9] * data[7] * data[14] + data[13] * data[6] * data[11] - data[13] * data[7] * data[10];
		inv[1] = -data[1] * data[10] * data[15] + data[1] * data[11] * data[14] + data[9] * data[2] * data[15] - data[9] * data[3] * data[14] - data[13] * data[2] * data[11] + data[13] * data[3] * data[10];
		inv[2] = data[1] * data[6] * data[15] - data[1] * data[7] * data[14] - data[5] * data[2] * data[15] + data[5] * data[3] * data[14] + data[13] * data[2] * data[7] - data[13] * data[3] * data[6];
		inv[3] = -data[1] * data[6] * data[11] + data[1] * data[7] * data[10] + data[5] * data[2] * data[11] - data[5] * data[3] * data[10] - data[9] * data[2] * data[7] + data[9] * data[3] * data[6];
		inv[4] = -data[4] * data[10] * data[15] + data[4] * data[11] * data[14] + data[8] * data[6] * data[15] - data[8] * data[7] * data[14] - data[12] * data[6] * data[11] + data[12] * data[7] * data[10];
		inv[5] = data[0] * data[10] * data[15] - data[0] * data[11] * data[14] - data[8] * data[2] * data[15] + data[8] * data[3] * data[14] + data[12] * data[2] * data[11] - data[12] * data[3] * data[10];
		inv[6] = -data[0] * data[6] * data[15] + data[0] * data[7] * data[14] + data[4] * data[2] * data[15] - data[4] * data[3] * data[14] - data[12] * data[2] * data[7] + data[12] * data[3] * data[6];
		inv[7] = data[0] * data[6] * data[11] - data[0] * data[7] * data[10] - data[4] * data[2] * data[11] + data[4] * data[3] * data[10] + data[8] * data[2] * data[7] - data[8] * data[3] * data[6];
		inv[8] = data[4] * data[9] * data[15] - data[4] * data[11] * data[13] - data[8] * data[5] * data[15] + data[8] * data[7] * data[13] + data[12] * data[5] * data[11] - data[12] * data[7] * data[9];
		inv[9] = -data[0] * data[9] * data[15] + data[0] * data[11] * data[13] + data[8] * data[1] * data[15] - data[8] * data[3] * data[13] - data[12] * data[1] * data[11] + data[12] * data[3] * data[9];
		inv[10] = data[0] * data[5] * data[15] - data[0] * data[7] * data[13] - data[4] * data[1] * data[15] + data[4] * data[3] * data[13] + data[12] * data[1] * data[7] - data[12] * data[3] * data[5];
		inv[11] = -data[0] * data[5] * data[11] + data[0] * data[7] * data[9] + data[4] * data[1] * data[11] - data[4] * data[3] * data[9] - data[8] * data[1] * data[7] + data[8] * data[3] * data[5];
		inv[12] = -data[4] * data[9] * data[14] + data[4] * data[10] * data[13] + data[8] * data[5] * data[14] - data[8] * data[6] * data[13] - data[12] * data[5] * data[10] + data[12] * data[6] * data[9];
		inv[13] = data[0] * data[9] * data[14] - data[0] * data[10] * data[13] - data[8] * data[1] * data[14] + data[8] * data[2] * data[13] + data[12] * data[1] * data[10] - data[12] * data[2] * data[9];
		inv[14] = -data[0] * data[5] * data[14] + data[0] * data[6] * data[13] + data[4] * data[1] * data[14] - data[4] * data[2] * data[13] - data[12] * data[1] * data[6] + data[12] * data[2] * data[5];
		inv[15] = data[0] * data[5] * data[10] - data[0] * data[6] * data[9] - data[4] * data[1] * data[10] + data[4] * data[2] * data[9] + data[8] * data[1] * data[6] - data[8] * data[2] * data[5];

		const T det = data[0] * inv[0] + data[1] * inv[4] + data[2] * inv[8] + data[3] * inv[12];
		if (Math::Equals(det, T(0)))
		{
			if (succeeded != nullptr)
			{
				*succeeded = false;
			}
			return Identity();
		}

		const T invDet = 1 / det;
		for (U32 i = 0; i < 16; i++)
		{
			inv[i] *= invDet;
		}

		if (succeeded != nullptr)
		{
			*succeeded = true;
		}
		return Matrix4<T>(inv[0], inv[1], inv[2], inv[3], inv[4], inv[5], inv[6], inv[7], inv[8], inv[9], inv[10], inv[11], inv[12], inv[13], inv[14], inv[15]);
	}

	constexpr Matrix4<T>& Transpose()
	{
		T temp = data[1];
		data[1] = data[4];
		data[4] = temp;
		temp = data[2];
		data[2] = data[8];
		data[8] = temp;
		temp = data[3];
		data[3] = data[12];
		data[12] = temp;
		temp = data[6];
		data[6] = data[9];
		data[9] = temp;
		temp = data[7];
		data[7] = data[13];
		data[13] = temp;
		temp = data[11];
		data[11] = data[14];
		data[14] = temp;
		return *this;
	}
	constexpr Matrix4<T> Transposed() const { return Matrix4<T>(data[0], data[4], data[8], data[12], data[1], data[5], data[9], data[13], data[2], data[6], data[10], data[14], data[3], data[7], data[11], data[15]); }

	constexpr Vector3<T> TransformPoint(const Vector3<T>& point) const
	{
		Vector3<T> out;
		out.x = Vector3<T>(data[0], data[4], data[8]).DotProduct(point) + data[12];
		out.y = Vector3<T>(data[1], data[5], data[9]).DotProduct(point) + data[13];
		out.z = Vector3<T>(data[2], data[6], data[10]).DotProduct(point) + data[14];
		return out;
	}
	constexpr Vector3<T> TransformDirection(const Vector3<T>& direction) const
	{
		Vector3<T> out;
		out.x = Vector3<T>(data[0], data[4], data[8]).DotProduct(direction);
		out.y = Vector3<T>(data[1], data[5], data[9]).DotProduct(direction);
		out.z = Vector3<T>(data[2], data[6], data[10]).DotProduct(direction);
		return out;
	}

	// Should it be applied to the rotation only or to the whole matrix (including translation) ? Probably better to not exists to avoid confusions
	/*
	constexpr Matrix4<T> operator*(const Matrix3<T>& m) const
	{
		Matrix4<T> out;
		const Vector3<T> r0(GetRow(0).xyz());
		const Vector3<T> r1(GetRow(1).xyz());
		const Vector3<T> r2(GetRow(2).xyz());
		const Vector3<T> r3(GetRow(3).xyz());
		const Vector3<T> c0(m.GetColumn(0));
		const Vector3<T> c1(m.GetColumn(1));
		const Vector3<T> c2(m.GetColumn(2));
		out.data[0] = c0.DotProduct(r0);
		out.data[1] = c1.DotProduct(r0);
		out.data[2] = c2.DotProduct(r0);
		out.data[3] = data[3];
		out.data[4] = c0.DotProduct(r1);
		out.data[5] = c1.DotProduct(r1);
		out.data[6] = c2.DotProduct(r1);
		out.data[7] = data[7];
		out.data[8] = c0.DotProduct(r2);
		out.data[9] = c1.DotProduct(r2);
		out.data[10] = c2.DotProduct(r2);
		out.data[11] = data[11];
		out.data[12] = c0.DotProduct(r3);
		out.data[13] = c1.DotProduct(r3);
		out.data[14] = c2.DotProduct(r3);
		out.data[15] = data[15];
		return out;
	}
	constexpr Matrix4<T>& operator*=(const Matrix3<T>& m)
	{
		const Vector3<T> r0(GetRow(0).xyz());
		const Vector3<T> r1(GetRow(1).xyz());
		const Vector3<T> r2(GetRow(2).xyz());
		const Vector3<T> r3(GetRow(3).xyz());
		const Vector3<T> c0(m.GetColumn(0));
		const Vector3<T> c1(m.GetColumn(1));
		const Vector3<T> c2(m.GetColumn(2));
		data[0] = c0.DotProduct(r0);
		data[1] = c1.DotProduct(r0);
		data[2] = c2.DotProduct(r0);
		data[4] = c0.DotProduct(r1);
		data[5] = c1.DotProduct(r1);
		data[6] = c2.DotProduct(r1);
		data[8] = c0.DotProduct(r2);
		data[9] = c1.DotProduct(r2);
		data[10] = c2.DotProduct(r2);
		data[12] = c0.DotProduct(r3);
		data[13] = c1.DotProduct(r3);
		data[14] = c2.DotProduct(r3);
		return *this;
	}
	*/

	constexpr Vector3<T> GetTranslation() const { return Vector3<T>(data[12], data[13], data[14]); }
	constexpr Matrix4<T>& SetTranslation(const T& tx, const T& ty, const T& tz) { data[12] = tx; data[13] = ty; data[14] = tz; return *this; }
	constexpr Matrix4<T>& SetTranslation(const Vector3<T>& translation) { data[12] = translation.x; data[13] = translation.y; data[14] = translation.z; data[15] = T(1); return *this; }
	constexpr Matrix4<T>& ApplyTranslation(const T& tx, const T& ty, const T& tz) { data[12] += tx; data[13] += ty; data[14] += tz; return *this; }
	constexpr Matrix4<T>& ApplyTranslation(const Vector3<T>& translation) { data[12] += translation.x; data[13] += translation.y; data[14] += translation.z; return *this; }

	inline bool IsScaled() const { return GetScale() != Vector3<T>::Unit(); }
	inline bool HasUniformScale() const { const Vector3<T> scale = GetScale(); return scale.x == scale.y && scale.x == scale.z; }
	inline Vector3<T> GetScale() const
	{ 
		return Vector3<T>(Math::FastSqrt(data[0] * data[0] + data[1] * data[1] + data[2] * data[2]),
			Math::FastSqrt(data[4] * data[4] + data[5] * data[5] + data[6] * data[6]),
			Math::FastSqrt(data[8] * data[8] + data[9] * data[9] + data[10] * data[10])); 
	}
	inline Matrix4<T>& SetScale(const Vector3<T>& scale)
	{
		const Vector3<T> scaleChange = scale / GetScale();
		data[0] *= scaleChange.x;
		data[1] *= scaleChange.x;
		data[2] *= scaleChange.x;
		data[4] *= scaleChange.y;
		data[5] *= scaleChange.y;
		data[6] *= scaleChange.y;
		data[8] *= scaleChange.z;
		data[9] *= scaleChange.z;
		data[10] *= scaleChange.z;
		return *this;
	}
	inline Matrix4<T>& SetScale(const T& sx, const T& sy, const T& sz) { return SetScale(Vector3<T>(sx, sy, sz)); }
	inline Matrix4<T>& SetScale(const T& s) { return SetScale(Vector3<T>(s)); }
	constexpr Matrix4<T>& ApplyScale(const Vector3<T>& scale) 
	{ 
		data[0] *= scale.x; 
		data[1] *= scale.y; 
		data[2] *= scale.z;
		data[4] *= scale.x;
		data[5] *= scale.y; 
		data[6] *= scale.z; 
		data[8] *= scale.x; 
		data[9] *= scale.y; 
		data[10] *= scale.z;
		data[12] *= scale.x; 
		data[13] *= scale.y;
		data[14] *= scale.z;
		return *this; 
	}
	constexpr Matrix4<T>& ApplyScale(const T& sx, const T& sy, const T& sz) { return ApplyScale(Vector3<T>(sx, sy, sz)); }
	constexpr Matrix4<T>& ApplyScale(const T& s) { return ApplyScale(Vector3<T>(s)); }

	inline Matrix3<T> GetRotation() const
	{
		const Vector3<T> scale = GetScale();
		Matrix3<T> result;
		result[0] = data[0] / scale.x;
		result[1] = data[1] / scale.x;
		result[2] = data[2] / scale.x;
		result[3] = data[4] / scale.y;
		result[4] = data[5] / scale.y;
		result[5] = data[6] / scale.y;
		result[6] = data[8] / scale.z;
		result[7] = data[9] / scale.z;
		result[8] = data[10] / scale.z;
		return result;
	}
	inline Matrix4<T>& SetRotation(const Matrix3<T>& rotation)
	{
		const Vector3<T> scale = GetScale();
		data[0] = rotation[0] * scale.x;
		data[1] = rotation[1] * scale.x;
		data[2] = rotation[2] * scale.x;
		data[4] = rotation[3] * scale.y;
		data[5] = rotation[4] * scale.y;
		data[6] = rotation[5] * scale.y;
		data[8] = rotation[6] * scale.z;
		data[9] = rotation[7] * scale.z;
		data[10] = rotation[8] * scale.z;
		return *this;
	}
	constexpr Matrix4<T>& ApplyRotation(const Matrix3<T>& rotation)
	{
		// TODO : Can be optimized a bit as we know the last col is (0,0,0,1)
		*this *= Rotation(rotation);
		return *this;
	}

	inline Quaternion<T> GetQuaternion() const
	{
		return Quaternion<T>(GetRotation());
	}
	inline Matrix4<T>& SetQuaternion(const Quaternion<T>& quaternion)
	{
		return SetRotation(quaternion.ToMatrix3());
	}
	inline Matrix4<T>& ApplyQuaternion(const Quaternion<T>& quaternion)
	{
		return ApplyRotation(quaternion.ToMatrix3());
	}

	constexpr Vector3f GetForward() const { return TransformDirection(ENLIVE_DEFAULT_FORWARD); }
	constexpr Vector3f GetBackward() const { return TransformDirection(ENLIVE_DEFAULT_BACKWARD); }
	constexpr Vector3f GetUp() const { return TransformDirection(ENLIVE_DEFAULT_UP); }
	constexpr Vector3f GetDown() const { return TransformDirection(ENLIVE_DEFAULT_DOWN); }
	constexpr Vector3f GetLeft() const { return TransformDirection(ENLIVE_DEFAULT_LEFT); }
	constexpr Vector3f GetRight() const { return TransformDirection(ENLIVE_DEFAULT_RIGHT); }
	
	static constexpr Matrix4<T> Rotation(const Matrix3<T> & m) { return Matrix4<T>(m[0], m[1], m[2], T(0), m[3], m[4], m[5], T(0), m[6], m[7], m[8], T(0), T(0), T(0), T(0), T(1)); }
	static constexpr Matrix4<T> RotationX(const T & angle) { return RotationX(Vector2<T>(Math::Cos(angle), Math::Sin(angle))); }
	static constexpr Matrix4<T> RotationY(const T & angle) { return RotationY(Vector2<T>(Math::Cos(angle), Math::Sin(angle))); }
	static constexpr Matrix4<T> RotationZ(const T & angle) { return RotationZ(Vector2<T>(Math::Cos(angle), Math::Sin(angle))); }
	static constexpr Matrix4<T> RotationX(const Vector2<T> & v) { return Matrix4<T>(T(1), T(0), T(0), T(0), T(0), v.x, -v.y, T(0), T(0), v.y, v.x, T(0), T(0), T(0), T(0), T(1)); }
	static constexpr Matrix4<T> RotationY(const Vector2<T> & v) { return Matrix4<T>(v.x, T(0), v.y, T(0), T(0), T(1), T(0), T(0), -v.y, T(0), v.x, T(0), T(0), T(0), T(0), T(1)); }
	static constexpr Matrix4<T> RotationZ(const Vector2<T> & v) { return Matrix4<T>(v.x, -v.y, T(0), T(0), v.y, v.x, T(0), T(0), T(0), T(0), T(1), T(0), T(0), T(0), T(0), T(1)); }
	static constexpr Matrix4<T> Scale(const Vector3<T> & scale) { return Matrix4<T>(scale.x, T(0), T(0), T(0), T(0), scale.y, T(0), T(0), T(0), T(0), scale.z, T(0), T(0), T(0), T(0), T(1)); }
	static constexpr Matrix4<T> Scale(const T & sx, const T & sy, const T & sz) { return Matrix4<T>(sx, T(0), T(0), T(0), T(0), sy, T(0), T(0), T(0), T(0), sz, T(0), T(0), T(0), T(0), T(1)); }
	static constexpr Matrix4<T> Scale(const T & s) { return Matrix4<T>(s, T(0), T(0), T(0), T(0), s, T(0), T(0), T(0), T(0), s, T(0), T(0), T(0), T(0), T(1)); }
	static constexpr Matrix4<T> Translation(const Vector3<T> & translation) { return Matrix4<T>(T(1), T(0), T(0), T(0), T(0), T(1), T(0), T(0), T(0), T(0), T(1), T(0), translation.x, translation.y, translation.z, T(1)); }
	static constexpr Matrix4<T> Translation(const T & tx, const T & ty, const T & tz) { return Matrix4<T>(T(1), T(0), T(0), T(0), T(0), T(1), T(0), T(0), T(0), T(0), T(1), T(0), tx, ty, tz, T(1)); }

	static constexpr Matrix4<T> Transform(const Vector3<T>& translation, const Matrix3<T>& rotation, const Vector3<T>& scale = Vector3<T>(1.0f))
	{
		Matrix4<T> m;
		m[0] = scale.x * rotation[0];
		m[1] = scale.x * rotation[1];
		m[2] = scale.x * rotation[2];
		m[4] = scale.y * rotation[3];
		m[5] = scale.y * rotation[4];
		m[6] = scale.y * rotation[5];
		m[8] = scale.z * rotation[6];
		m[9] = scale.z * rotation[7];
		m[10] = scale.z * rotation[8];
		m[12] = translation.x;
		m[13] = translation.y;
		m[14] = translation.z;
		return m;
	}
	static constexpr Matrix4<T> Transform(const Vector3<T>& translation, const Quaternion<T>& rotation, const Vector3<T>& scale = Vector3<T>(1.0f))
	{
		return Transform(translation, rotation.ToMatrix3(), scale);
	}

	static constexpr Matrix4<T> Perspective(const T & fov, const T & aspect, const T & nearPlane, const T & farPlane, bool homogeneousNdc, Math::Handedness handedness)
	{
		const T tanHalfFov = Math::Tan(T(0.5) * fov);
		const T invZDiff = T(1) / (farPlane - nearPlane);

		const T _0 = T(1) / (aspect * tanHalfFov);
		const T _5 = T(1) / tanHalfFov;
		const T _11 = (handedness == Math::Handedness::Right ? T(-1) : T(1));
		const T _10 = _11 * (homogeneousNdc ? farPlane + nearPlane : farPlane) * invZDiff;
		const T _14 = -(homogeneousNdc ? T(2) * farPlane * nearPlane : farPlane * nearPlane) * invZDiff;

		return Matrix4<T>(_0, T(0), T(0), T(0), T(0), _5, T(0), T(0), T(0), T(0), _10, _11, T(0), T(0), _14, T(0));
	}

	static constexpr Matrix4<T> Perspective(const T & fov, const T & width, const T & height, const T & nearPlane, const T & farPlane, bool homogeneousNdc, Math::Handedness handedness)
	{
		const T h = Math::Cos(T(0.5) * fov) / Math::Sin(T(0.5) * fov);
		const T w = h * height / width;
		const T invZDiff = T(1) / (farPlane - nearPlane);

		const T _11 = (handedness == Math::Handedness::Right ? T(-1) : T(1));
		const T _10 = _11 * (homogeneousNdc ? farPlane + nearPlane : farPlane) * invZDiff;
		const T _14 = -(homogeneousNdc ? T(2) * farPlane * nearPlane : farPlane * nearPlane) * invZDiff;

		return Matrix4<T>(w, T(0), T(0), T(0), T(0), h, T(0), T(0), T(0), T(0), _10, _11, T(0), T(0), _14, T(0));
	}

	static constexpr Matrix4<T> Orthographic(const T & left, const T & right, const T & top, const T & bottom, const T & nearPlane, const T & farPlane, bool homogeneousNdc, Math::Handedness handedness)
	{
		const T invWidth = T(1) / (right - left);
		const T invHeight = T(1) / (top - bottom);
		const T invZDiff = T(1) / (farPlane - nearPlane);

		const T _10 = (homogeneousNdc ? T(2) : T(1)) * invZDiff;
		const T _12 = -(right + left) * invWidth;
		const T _13 = -(top + bottom) * invHeight;
		const T _14 = -(homogeneousNdc ? -(nearPlane + farPlane) : nearPlane) * invZDiff;

		return Matrix4<T>(T(2) * invWidth, T(0), T(0), T(0), T(0), T(2) * invHeight, T(0), T(0), T(0), T(0), handedness == Math::Handedness::Right ? -_10 : _10, T(0), _12, _13, _14, T(1));
	}

	static inline Matrix4<T> LookAt(const Vector3<T> & eye, const Vector3<T> & target, const Vector3<T> & up, Math::Handedness handedness)
	{
		const Vector3<T> f((target - eye).Normalized());
		const Vector3<T> s(handedness == Math::Handedness::Right ? Vector3f::CrossProduct(f, up).Normalized() : Vector3f::CrossProduct(up, f).Normalized());
		const Vector3<T> u(handedness == Math::Handedness::Right ? Vector3f::CrossProduct(s, f) : Vector3f::CrossProduct(f, s));
		const T factor = (handedness == Math::Handedness::Right ? -1.0f : 1.0f);

		return Matrix4<T>(s.x, u.x, factor * f.x, T(0), s.y, u.y, factor * f.y, T(0), s.z, u.z, factor * f.z, T(0), -s.DotProduct(eye), -u.DotProduct(eye), -factor * f.DotProduct(eye), T(1));
	}

	static constexpr Matrix4<T> Zero() { return Matrix4<T>(T(0), T(0), T(0), T(0), T(0), T(0), T(0), T(0), T(0), T(0), T(0), T(0), T(0), T(0), T(0), T(0)); }
	static constexpr Matrix4<T> Identity() { return Matrix4<T>(T(1), T(0), T(0), T(0), T(0), T(1), T(0), T(0), T(0), T(0), T(1), T(0), T(0), T(0), T(0), T(1)); }

	constexpr T* GetData() { return data; }
	constexpr const T* GetData() const { return data; }

private:
	T data[16];
};

typedef Matrix4<F32> Matrix4f;

typedef Matrix4f mat4; // GLSL-like

} // namespace en

#endif // ENLIVE_MODULE_MATH
