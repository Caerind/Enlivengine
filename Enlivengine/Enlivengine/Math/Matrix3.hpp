#pragma once

#include <Enlivengine/Config.hpp>

#ifdef ENLIVE_MODULE_MATH

#include <Enlivengine/Math/Vector3.hpp>

// TODO : Constexpr memcpy, swap

namespace en
{

// 
// Matrix layout :    
// 
// 0 1 2     a11 a12 a13
// 3 4 5  =  a21 a22 a23
// 6 7 8     a31 a32 a33
//

template <typename T>
class Matrix3
{
public:
	static constexpr I32 Rows{ 3 };
	static constexpr I32 Columns{ 3 };
	static constexpr I32 Elements{ Rows * Columns };

	constexpr Matrix3() : data{ T(1), T(0), T(0), T(0), T(1), T(0), T(0), T(0), T(1) } {}
	constexpr Matrix3(const Matrix3<T>& m) : data{ m.data[0], m.data[1], m.data[2], m.data[3], m.data[4], m.data[5], m.data[6], m.data[7], m.data[8] } {}
	template <typename U>
	constexpr Matrix3(const Matrix3<U>& m) : data{ static_cast<T>(m.data[0]), static_cast<T>(m.data[1]), static_cast<T>(m.data[2]), static_cast<T>(m.data[3]), static_cast<T>(m.data[4]), static_cast<T>(m.data[5]), static_cast<T>(m.data[6]), static_cast<T>(m.data[7]), static_cast<T>(m.data[8]) } {}
	constexpr Matrix3(const T* a) : data{ a[0], a[1], a[2], a[3], a[4], a[5], a[6], a[7], a[8] } {}
	constexpr Matrix3(const T& a11, const T& a12, const T& a13, const T& a21, const T& a22, const T& a23, const T& a31, const T& a32, const T& a33) : data{ a11, a12, a13, a21, a22, a23, a31, a32, a33 } {}
	~Matrix3() = default;

	constexpr Matrix3<T>& Set(const Matrix3<T> & m)
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
		return *this;
	}
	template <typename U>
	constexpr Matrix3<T>& Set(const Matrix3<U> & m)
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
		return *this;
	}
	constexpr Matrix3<T>& Set(const T * a)
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
		return *this;
	}
	constexpr Matrix3<T>& Set(const T & a11, const T & a12, const T & a13, const T & a21, const T & a22, const T & a23, const T & a31, const T & a32, const T & a33)
	{
		data[0] = a11;
		data[1] = a12;
		data[2] = a13;
		data[3] = a21;
		data[4] = a22;
		data[5] = a23;
		data[6] = a31;
		data[7] = a32;
		data[8] = a33;
		return *this;
	}

	constexpr T& operator[](U32 i) { return data[i]; }
	constexpr const T& operator[](U32 i) const { return data[i]; }
	constexpr T& operator()(U32 row, U32 column) { return data[column + Columns * row]; }
	constexpr const T& operator()(U32 row, U32 column) const { return data[column + Columns * row]; }

	constexpr Vector3<T> GetColumn(U32 columnIndex) const { return Vector3<T>(data[columnIndex], data[columnIndex + Columns], data[columnIndex + Columns * 2]); }
	constexpr Vector3<T> GetRow(U32 rowIndex) const { const U32 r = Columns * rowIndex; return Vector3<T>(data[r], data[r + 1], data[r + 2]); }
	constexpr Matrix3<T>& SetColumn(U32 columnIndex, const Vector3<T> & column)
	{
		data[columnIndex] = column.x;
		data[columnIndex + Columns * 1] = column.y;
		data[columnIndex + Columns * 2] = column.z;
		return *this;
	}
	constexpr Matrix3<T>& SetRow(U32 rowIndex, const Vector3<T> & row)
	{
		const U32 r = Columns * rowIndex;
		data[r] = row.x;
		data[r + 1] = row.y;
		data[r + 2] = row.z;
		return *this;
	}

	constexpr Matrix3<T>& operator=(const Matrix3<T> & m) { return Set(m); }
	constexpr const Matrix3<T>& operator+() const { return *this; }
	constexpr Matrix3<T> operator-() const { return Matrix3<T>(-data[0], -data[1], -data[2], -data[3], -data[4], -data[5], -data[6], -data[7], -data[8]); }

	constexpr Matrix3<T> operator+(const Matrix3<T> & m) const { return Matrix3<T>(data[0] + m.data[0], data[1] + m.data[1], data[2] + m.data[2], data[3] + m.data[3], data[4] + m.data[4], data[5] + m.data[5], data[6] + m.data[6], data[7] + m.data[7], data[8] + m.data[8]); }
	constexpr Matrix3<T> operator-(const Matrix3<T> & m) const { return Matrix3<T>(data[0] - m.data[0], data[1] - m.data[1], data[2] - m.data[2], data[3] - m.data[3], data[4] - m.data[4], data[5] - m.data[5], data[6] - m.data[6], data[7] - m.data[7], data[8] - m.data[8]); }
	constexpr Matrix3<T> operator*(const Matrix3<T> & m) const
	{
		Matrix3<T> out;
		const Vector3<T> r0(GetRow(0));
		const Vector3<T> r1(GetRow(1));
		const Vector3<T> r2(GetRow(2));
		const Vector3<T> c0(m.GetColumn(0));
		const Vector3<T> c1(m.GetColumn(1));
		const Vector3<T> c2(m.GetColumn(2));
		out.data[0] = c0.DotProduct(r0);
		out.data[1] = c1.DotProduct(r0);
		out.data[2] = c2.DotProduct(r0);
		out.data[3] = c0.DotProduct(r1);
		out.data[4] = c1.DotProduct(r1);
		out.data[5] = c2.DotProduct(r1);
		out.data[6] = c0.DotProduct(r2);
		out.data[7] = c1.DotProduct(r2);
		out.data[8] = c2.DotProduct(r2);
		return out;
	}
	constexpr Matrix3<T>& operator+=(const Matrix3<T> & m) { data[0] += m.data[0]; data[1] += m.data[1]; data[2] += m.data[2]; data[3] += m.data[3]; data[4] += m.data[4]; data[5] += m.data[5]; data[6] += m.data[6]; data[7] += m.data[7]; data[8] += m.data[8]; return *this; }
	constexpr Matrix3<T>& operator-=(const Matrix3<T> & m) { data[0] -= m.data[0]; data[1] -= m.data[1]; data[2] -= m.data[2]; data[3] -= m.data[3]; data[4] -= m.data[4]; data[5] -= m.data[5]; data[6] -= m.data[6]; data[7] -= m.data[7]; data[8] -= m.data[8]; return *this; }
	constexpr Matrix3<T>& operator*=(const Matrix3<T> & m)
	{
		const Vector3<T> r0(GetRow(0));
		const Vector3<T> r1(GetRow(1));
		const Vector3<T> r2(GetRow(2));
		const Vector3<T> c0(m.GetColumn(0));
		const Vector3<T> c1(m.GetColumn(1));
		const Vector3<T> c2(m.GetColumn(2));
		data[0] = c0.DotProduct(r0);
		data[1] = c1.DotProduct(r0);
		data[2] = c2.DotProduct(r0);
		data[3] = c0.DotProduct(r1);
		data[4] = c1.DotProduct(r1);
		data[5] = c2.DotProduct(r1);
		data[6] = c0.DotProduct(r2);
		data[7] = c1.DotProduct(r2);
		data[8] = c2.DotProduct(r2);
		return *this;
	}

	constexpr Matrix3<T> operator*(const T & s) const { return Matrix3<T>(data[0] * s, data[1] * s, data[2] * s, data[3] * s, data[4] * s, data[5] * s, data[6] * s, data[7] * s, data[8] * s); }
	constexpr Matrix3<T> operator/(const T & s) const { const T inv = T(1) / s;  return Matrix3<T>(data[0] * inv, data[1] * inv, data[2] * inv, data[3] * inv, data[4] * inv, data[5] * inv, data[6] * inv, data[7] * inv, data[8] * inv); }
	constexpr Matrix3<T>& operator*=(const T & s) { data[0] *= s; data[1] *= s; data[2] *= s; data[3] *= s; data[4] *= s; data[5] *= s; data[6] *= s; data[7] *= s; data[8] *= s; return *this; }
	constexpr Matrix3<T>& operator/=(const T & s) { const T inv = T(1) / s; data[0] *= inv; data[1] *= inv; data[2] *= inv; data[3] *= inv; data[4] *= inv; data[5] *= inv; data[6] *= inv; data[7] *= inv; data[8] *= inv; return *this; }

	constexpr bool operator==(const Matrix3<T> & m) const
	{
		return Math::Equals(data[0], m.data[0])
			&& Math::Equals(data[1], m.data[1])
			&& Math::Equals(data[2], m.data[2])
			&& Math::Equals(data[3], m.data[3])
			&& Math::Equals(data[4], m.data[4])
			&& Math::Equals(data[5], m.data[5])
			&& Math::Equals(data[6], m.data[6])
			&& Math::Equals(data[7], m.data[7])
			&& Math::Equals(data[8], m.data[8]);
	}
	constexpr bool operator!=(const Matrix3<T> & m) const { return !operator==(m); }
	constexpr bool IsIdentity() const { return operator==(Identity()); }

	constexpr bool IsOrthonormal() const
	{
		return Math::Equals(T(1), GetRow(0).GetSquaredLength())
			&& Math::Equals(T(1), GetRow(1).GetSquaredLength())
			&& Math::Equals(T(1), GetRow(2).GetSquaredLength())
			&& Math::Equals(T(1), GetColumn(0).GetSquaredLength())
			&& Math::Equals(T(1), GetColumn(1).GetSquaredLength())
			&& Math::Equals(T(1), GetColumn(2).GetSquaredLength());
	}

	constexpr T GetTrace() const { return data[0] + data[4] + data[8]; }
	constexpr T GetDeterminant() const
	{
		const T det11 = data[4] * data[8] - data[5] * data[7];
		const T det12 = data[3] * data[8] - data[5] * data[6];
		const T det13 = data[3] * data[7] - data[4] * data[6];
		return data[0] * det11 - data[1] * det12 + data[2] * det13;
	}

	constexpr Matrix3<T>& Inverse(bool* succeeded = nullptr)
	{
		const T det11 = data[4] * data[8] - data[5] * data[7];
		const T det12 = data[3] * data[8] - data[5] * data[6];
		const T det13 = data[3] * data[7] - data[4] * data[6];
		const T det = data[0] * det11 - data[1] * det12 + data[2] * det13;
		if (Math::Equals(det, T(0)))
		{
			if (succeeded != nullptr)
			{
				*succeeded = false;
			}
			return *this;
		}

		const T invDet = T(1) / det;

		T inv[9];
		inv[0] = det11 * invDet;
		inv[1] = (data[2] * data[7] - data[1] * data[8]) * invDet;
		inv[2] = (data[1] * data[5] - data[2] * data[4]) * invDet;
		inv[3] = -det12 * invDet;
		inv[4] = (data[0] * data[8] - data[2] * data[6]) * invDet;
		inv[5] = (data[2] * data[3] - data[0] * data[5]) * invDet;
		inv[6] = det13 * invDet;
		inv[7] = (data[1] * data[6] - data[0] * data[7]) * invDet;
		inv[8] = (data[0] * data[4] - data[1] * data[3]) * invDet;

		if (succeeded != nullptr)
		{
			*succeeded = true;
		}
		return Set(inv);
	}
	constexpr Matrix3<T> Inversed(bool* succeeded = nullptr) const
	{
		const T det11 = data[4] * data[8] - data[5] * data[7];
		const T det12 = data[3] * data[8] - data[5] * data[6];
		const T det13 = data[3] * data[7] - data[4] * data[6];
		const T det = data[0] * det11 - data[1] * det12 + data[2] * det13;
		if (Math::Equals(det, T(0)))
		{
			if (succeeded != nullptr)
			{
				*succeeded = false;
			}
			return Identity();
		}

		const T invDet = T(1) / det;

		Matrix3<T> out;
		out.data[0] = det11 * invDet;
		out.data[1] = (data[2] * data[7] - data[1] * data[8]) * invDet;
		out.data[2] = (data[1] * data[5] - data[2] * data[4]) * invDet;
		out.data[3] = -det12 * invDet;
		out.data[4] = (data[0] * data[8] - data[2] * data[6]) * invDet;
		out.data[5] = (data[2] * data[3] - data[0] * data[5]) * invDet;
		out.data[6] = det13 * invDet;
		out.data[7] = (data[1] * data[6] - data[0] * data[7]) * invDet;
		out.data[8] = (data[0] * data[4] - data[1] * data[3]) * invDet;

		if (succeeded != nullptr)
		{
			*succeeded = true;
		}
		return out;
	}

	constexpr Matrix3<T>& Transpose()
	{
		T temp = data[1];
		data[1] = data[3];
		data[3] = temp;
		temp = data[2];
		data[2] = data[6];
		data[6] = temp;
		temp = data[5];
		data[5] = data[7];
		data[7] = temp;
		return *this;
	}
	constexpr Matrix3<T> Transposed() const { return Matrix3<T>(data[0], data[3], data[6], data[1], data[4], data[7], data[2], data[5], data[8]); }

	constexpr Vector3<T> TransformDirection(const Vector3<T>& direction) const
	{
		Vector3<T> out;
		out.x = GetColumn(0).DotProduct(direction);
		out.y = GetColumn(1).DotProduct(direction);
		out.z = GetColumn(2).DotProduct(direction);
		return out;
	}

	static constexpr Matrix3<T> RotationX(const T & angle) { return RotationX(Vector2<T>(Math::Cos(angle), Math::Sin(angle))); }
	static constexpr Matrix3<T> RotationY(const T & angle) { return RotationY(Vector2<T>(Math::Cos(angle), Math::Sin(angle))); }
	static constexpr Matrix3<T> RotationZ(const T & angle) { return RotationZ(Vector2<T>(Math::Cos(angle), Math::Sin(angle))); }
	static constexpr Matrix3<T> RotationX(const Vector2<T> & v) { return Matrix3<T>(T(1), T(0), T(0), T(0), v.x, -v.y, T(0), v.y, v.x); }
	static constexpr Matrix3<T> RotationY(const Vector2<T> & v) { return Matrix3<T>(v.x, T(0), v.y, T(0), T(1), T(0), -v.y, T(0), v.x); }
	static constexpr Matrix3<T> RotationZ(const Vector2<T> & v) { return Matrix3<T>(v.x, -v.y, T(0), v.y, v.x, T(0), T(0), T(0), T(1)); }

	static constexpr Matrix3<T> Zero() { return Matrix3<T>(T(0), T(0), T(0), T(0), T(0), T(0), T(0), T(0), T(0)); }
	static constexpr Matrix3<T> Identity() { return Matrix3<T>(T(1), T(0), T(0), T(0), T(1), T(0), T(0), T(0), T(1)); }

	constexpr T* GetData() { return data; }
	constexpr const T* GetData() const { return data; }

private:
	T data[9];
};

typedef Matrix3<F32> Matrix3f;

typedef Matrix3f mat3; // GLSL-like

} // namespace en

#endif // ENLIVE_MODULE_MATH
