#pragma once

#include <Enlivengine/Math/Utilities.hpp>

// TODO : constexpr

namespace en
{

template <typename T> class Matrix3;
template <typename T> class Matrix4;
template <typename T> class Vector3;
template <typename T> class Vector4;

template <typename T>
class Quaternion
{
public:
	inline Quaternion();
	inline Quaternion(const Quaternion<T>& q);
	template <typename U>
	explicit inline Quaternion(const Quaternion<U>& q);
	explicit inline Quaternion(const T* a);
	explicit inline Quaternion(const Vector4<T>& v);
	inline Quaternion(const T& x, const T& y, const T& z, const T& w);
	inline Quaternion(const Vector3<T>& v, const T& s);
	inline Quaternion(const Vector3<T>& eulerAngles);
	inline Quaternion(const T& angle, const Vector3<T>& axis);
	inline Quaternion(const Matrix3<T>& matrix);
	~Quaternion() = default;

	inline Quaternion<T>& set(const Quaternion<T>& q);
	template <typename U>
	inline Quaternion<T>& set(const Quaternion<U>& q);
	inline Quaternion<T>& set(const T* a);
	inline Quaternion<T>& set(const Vector4<T>& v);
	inline Quaternion<T>& set(const T& x, const T& y, const T& z, const T& w);
	inline Quaternion<T>& set(const Vector3<T>& v, const T& s);
	inline Quaternion<T>& set(const Vector3<T>& eulerAngles);
	inline Quaternion<T>& set(const T& angle, const Vector3<T>& axis);
	inline Quaternion<T>& set(const Matrix3<T>& matrix);

	inline T& operator()(U32 i);
	inline const T& operator()(U32 i) const;
	inline T& operator[](U32 i);
	inline const T& operator[](U32 i) const;

	inline Quaternion<T>& operator=(const Quaternion<T>& q);

	inline Quaternion<T> operator*(const Quaternion<T>& q) const;
	inline Quaternion<T>& operator*=(const Quaternion<T>& q);
	inline Quaternion<T> operator*(const T& s) const;
	inline Quaternion<T>& operator*=(const T& s);
	inline Vector3<T> operator*(const Vector3<T>& vector) const;

	inline bool operator==(const Quaternion<T>& q) const;
	inline bool operator!=(const Quaternion<T>& q) const;

	inline Quaternion<T>& conjugate();
	inline Quaternion<T> conjugated();

	inline Quaternion<T>& inverse();
	inline Quaternion<T> inversed();

	inline T dotProduct(const Quaternion<T>& q) const;
	inline T getSquaredLength() const;
	inline T getLength() const;
	inline Quaternion<T>& normalize(T* oldLength = nullptr);
	inline Quaternion<T> normalized(T* oldLength = nullptr);
	inline Quaternion<T>& ensureNormalized();

	inline void toEulerAngles(Vector3<T>& vector) const;
	inline Vector3<T> toEulerAngles() const;
	inline void toAngleAxis(T& angle, Vector3<T>& axis) const;
	inline void toMatrix3(Matrix3<T>& matrix) const;
	inline Matrix3<T> toMatrix3() const;
	inline void toMatrix4(Matrix4<T>& matrix) const;
	inline Matrix4<T> toMatrix4() const;
	inline Vector4<T> toVector4() const;

	inline Quaternion<T>& fromEulerAngles(const Vector3<T>& vector);
	inline Quaternion<T>& fromAngleAxis(const T& angle, const Vector3<T>& axis);
	inline Quaternion<T>& fromMatrix3(const Matrix3<T>& matrix);
	inline Quaternion<T>& fromVector4(const Vector4<T>& vector);

	static inline Quaternion<T> slerp(const Quaternion<T>& start, const Quaternion<T>& end, const T& percent);

	static inline Quaternion<T> rotateFromTo(const Vector3<T>& start, const Vector3<T>& end);
	static inline Quaternion<T> rotateFromTo(const Vector3<T>& start, const Vector3<T>& end, const Vector3<T>& axis);

	inline Quaternion<T>& makeIdentity();

	static const Quaternion<T> identity;

	Vector3<T> v;
	T s;
};

template <typename T>
inline Quaternion<T> operator*(const T& s, const Quaternion<T>& quaternion);

} // namespace en

#include <Enlivengine/Math/Matrix3.hpp>
#include <Enlivengine/Math/Matrix4.hpp>
#include <Enlivengine/Math/Vector3.hpp>
#include <Enlivengine/Math/Vector4.hpp>

namespace en
{

template<typename T>
inline Quaternion<T>::Quaternion()
	: v()
	, s(T(0))
{
}

template<typename T>
inline Quaternion<T>::Quaternion(const Quaternion<T>& q)
	: v(q.v)
	, s(q.s)
{
}

template<typename T>
inline Quaternion<T>::Quaternion(const T* a)
	: v(a)
	, s(a[3])
{
}

template<typename T>
inline Quaternion<T>::Quaternion(const Vector4<T>& v)
	: v(v.x, v.y, v.z)
	, s(v.w)
{
}

template<typename T>
inline Quaternion<T>::Quaternion(const T& x, const T& y, const T& z, const T& w)
	: v(x, y, z)
	, s(w)
{
}

template<typename T>
inline Quaternion<T>::Quaternion(const Vector3<T>& v, const T& s)
	: v(v)
	, s(s)
{
}

template<typename T>
inline Quaternion<T>::Quaternion(const Vector3<T>& eulerAngles)
{
	Vector3<T> halfAngles(T(0.5) * eulerAngles);
	const T sx = Math::Sin(halfAngles.x);
	const T cx = Math::Cos(halfAngles.x);
	const T sy = Math::Sin(halfAngles.y);
	const T cy = Math::Cos(halfAngles.y);
	const T sz = Math::Sin(halfAngles.z);
	const T cz = Math::Cos(halfAngles.z);
	v.x = sx * cy * cz - cx * sy * sz;
	v.y = cx * sy * cz + sx * cy * sz;
	v.z = cx * cy * sz - sx * sy * cz;
	s = cx * cy * cz + sx * sy * sz;
}

template<typename T>
inline Quaternion<T>::Quaternion(const T& angle, const Vector3<T>& axis)
{
	const T halfAngle = angle * T(0.5);
	v.set(axis.normalized() * Math::Sin(halfAngle));
	s = Math::Cos(halfAngle);
}

template<typename T>
inline Quaternion<T>::Quaternion(const Matrix3<T>& matrix)
{
	const T trace = matrix.getTrace();
	if (trace > 0)
	{
		const T t = Math::Sqrt(trace + 1) * 2;
		const T overT = T(1) / t;
		v.x = (matrix[5] - matrix[7]) * overT;
		v.y = (matrix[6] - matrix[2]) * overT;
		v.z = (matrix[1] - matrix[3]) * overT;
		s = t * T(0.25);
	}
	else if (matrix[0] > matrix[4] && matrix[0] > matrix[8])
	{
		const T t = Math::Sqrt(matrix[0] - matrix[4] - matrix[8] + 1) * 2;
		const T overT = 1 / t;
		v.x = t * T(0.25);
		v.y = (matrix[3] + matrix[1]) * overT;
		v.z = (matrix[6] + matrix[2]) * overT;
		s = (matrix[5] - matrix[7]) * overT;
	}
	else if (matrix[4] > matrix[8])
	{
		const T t = Math::Sqrt(matrix[4] - matrix[0] - matrix[8] + 1) * 2;
		const T overT = 1 / t;
		v.x = (matrix[3] + matrix[1]) * overT;
		v.y = t * T(0.25);
		v.z = (matrix[5] + matrix[7]) * overT;
		s = (matrix[6] - matrix[2]) * overT;
	}
	else
	{
		const T t = Math::Sqrt(matrix[8] - matrix[0] - matrix[4] + 1) * 2;
		const T overT = 1 / t;
		v.x = (matrix[6] + matrix[2]) * overT;
		v.y = (matrix[5] + matrix[7]) * overT;
		v.z = t * T(0.25);
		s = (matrix[1] - matrix[3]) * overT;
	}
}

template<typename T>
inline Quaternion<T>& Quaternion<T>::set(const Quaternion<T>& q)
{
	v = q.v;
	s = q.s;
	return *this;
}

template<typename T>
inline Quaternion<T>& Quaternion<T>::set(const T* a)
{
	v.x = a[0];
	v.y = a[1];
	v.z = a[2];
	s = a[3];
	return *this;
}

template<typename T>
inline Quaternion<T>& Quaternion<T>::set(const Vector4<T>& v)
{
	this->v.x = v.x;
	this->v.y = v.y;
	this->v.z = v.z;
	s = v.w;
	return *this;
}

template<typename T>
inline Quaternion<T>& Quaternion<T>::set(const T& x, const T& y, const T& z, const T& w)
{
	v.set(x, y, z);
	s = w;
	return *this;
}

template<typename T>
inline Quaternion<T>& Quaternion<T>::set(const Vector3<T>& v, const T& s)
{
	this->v = v;
	this->s = s;
	return *this;
}

template<typename T>
inline Quaternion<T>& Quaternion<T>::set(const Vector3<T>& eulerAngles)
{
	Vector3<T> halfAngles(T(0.5) * eulerAngles);
	const T sx = Math::Sin(halfAngles.x);
	const T cx = Math::Cos(halfAngles.x);
	const T sy = Math::Sin(halfAngles.y);
	const T cy = Math::Cos(halfAngles.y);
	const T sz = Math::Sin(halfAngles.z);
	const T cz = Math::Cos(halfAngles.z);
	v.x = sx * cy * cz - cx * sy * sz;
	v.y = cx * sy * cz + sx * cy * sz;
	v.z = cx * cy * sz - sx * sy * cz;
	s = cx * cy * cz + sx * sy * sz;
	return *this;
}

template<typename T>
inline Quaternion<T>& Quaternion<T>::set(const T& angle, const Vector3<T>& axis)
{
	const T halfAngle = angle * T(0.5);
	v.set(axis.normalized() * Math::Sin(halfAngle));
	s = Math::Cos(halfAngle);
	return *this;
}

template<typename T>
inline Quaternion<T>& Quaternion<T>::set(const Matrix3<T>& matrix)
{
	const T trace = matrix.getTrace();
	if (trace > 0)
	{
		const T t = Math::Sqrt(trace + 1) * 2;
		const T overT = 1 / t;
		v.x = (matrix[5] - matrix[7]) * overT;
		v.y = (matrix[6] - matrix[2]) * overT;
		v.z = (matrix[1] - matrix[3]) * overT;
		s = t * T(0.25);
		return *this;
	}
	else if (matrix[0] > matrix[4] && matrix[0] > matrix[8])
	{
		const T t = Math::Sqrt(matrix[0] - matrix[4] - matrix[8] + 1) * 2;
		const T overT = 1 / t;
		v.x = t * T(0.25);
		v.y = (matrix[3] + matrix[1]) * overT;
		v.z = (matrix[6] + matrix[2]) * overT;
		s = (matrix[5] - matrix[7]) * overT;
		return *this;
	}
	else if (matrix[4] > matrix[8])
	{
		const T t = Math::Sqrt(matrix[4] - matrix[0] - matrix[8] + 1) * 2;
		const T overT = 1 / t;
		v.x = (matrix[3] + matrix[1]) * overT;
		v.y = t * T(0.25);
		v.z = (matrix[5] + matrix[7]) * overT;
		s = (matrix[6] - matrix[2]) * overT;
		return *this;
	}
	else
	{
		const T t = Math::Sqrt(matrix[8] - matrix[0] - matrix[4] + 1) * 2;
		const T overT = 1 / t;
		v.x = (matrix[6] + matrix[2]) * overT;
		v.y = (matrix[5] + matrix[7]) * overT;
		v.z = t * T(0.25);
		s = (matrix[1] - matrix[3]) * overT;
		return *this;
	}
}

template<typename T>
inline T& Quaternion<T>::operator()(U32 i)
{
	switch (i)
	{
	case 0: return v.x; break;
	case 1: return v.y; break;
	case 2: return v.z; break;
	default: break;
	}
	return s;
}

template<typename T>
inline const T& Quaternion<T>::operator()(U32 i) const
{
	switch (i)
	{
	case 0: return v.x; break;
	case 1: return v.y; break;
	case 2: return v.z; break;
	default: break;
	}
	return s;
}

template<typename T>
inline T & Quaternion<T>::operator[](U32 i)
{
	switch (i)
	{
	case 0: return v.x; break;
	case 1: return v.y; break;
	case 2: return v.z; break;
	default: break;
	}
	return s;
}

template<typename T>
inline const T & Quaternion<T>::operator[](U32 i) const
{
	switch (i)
	{
	case 0: return v.x; break;
	case 1: return v.y; break;
	case 2: return v.z; break;
	default: break;
	}
	return s;
}

template<typename T>
inline Quaternion<T>& Quaternion<T>::operator=(const Quaternion<T>& q)
{
	v = q.v;
	s = q.s;
	return *this;
}

template<typename T>
inline Quaternion<T> Quaternion<T>::operator*(const Quaternion<T>& q) const
{
	return Quaternion<T>(s * q.v + q.s * v + v.crossProduct(q.v), s * q.s - v.dotProduct(q.v));
}

template<typename T>
inline Quaternion<T>& Quaternion<T>::operator*=(const Quaternion<T>& q)
{
	Quaternion<T> p(*this);
	v = p.s * q.v;
	v += q.s * p.v;
	v += p.v.crossProduct(q.v);
	s = p.s * q.s - p.v.dotProduct(q.v);
	return *this;
}

template<typename T>
inline Quaternion<T> Quaternion<T>::operator*(const T& s) const
{
	T angle;
	Vector3<T> axis;
	toAngleAxis(angle, axis);
	angle *= s;
	angle *= T(0.5);
	return Quaternion<T>(axis.normalized() * Math::Sin(angle), Math::Cos(angle));
}

template<typename T>
inline Quaternion<T>& Quaternion<T>::operator*=(const T& s)
{
	T angle;
	Vector3<T> axis;
	toAngleAxis(angle, axis);
	angle *= s;
	angle *= T(0.5);
	v.set(axis.normalized() * Math::Sin(angle));
	s = Math::Cos(angle);
	return *this;
}

template<typename T>
inline Vector3<T> Quaternion<T>::operator*(const Vector3<T>& vector) const
{
	T ss = s + s;
	return ss * (v.crossProduct(vector)) + (ss * s - T(1)) * vector + T(2) * (v.dotProduct(vector)) * vector;
}

template<typename T>
inline bool Quaternion<T>::operator==(const Quaternion<T>& q) const
{
	if (!Math::Equals(s, q.s)) return false;
	return v == q.v;
}

template<typename T>
inline bool Quaternion<T>::operator!=(const Quaternion<T>& q) const
{
	return !operator==(q);
}

template<typename T>
inline Quaternion<T>& Quaternion<T>::conjugate()
{
	v *= T(-1);
	return *this;
}

template<typename T>
inline Quaternion<T> Quaternion<T>::conjugated()
{
	return Quaternion<T>(-v, s);
}

template<typename T>
inline Quaternion<T>& Quaternion<T>::inverse()
{
	conjugate();
	if (!Math::Equals(getSquaredLength(), T(1)))
	{
		const T sLength = getSquaredLength();
		v /= sLength;
		s /= sLength;
	}
	return *this;
}

template<typename T>
inline Quaternion<T> Quaternion<T>::inversed()
{
	return Quaternion<T>(*this).inverse();
}

template<typename T>
inline T Quaternion<T>::dotProduct(const Quaternion<T>& q) const
{
	return v.dotProduct(q.v) + s * q.s;
}

template<typename T>
inline T Quaternion<T>::getSquaredLength() const
{
	return dotProduct(*this);
}

template<typename T>
inline T Quaternion<T>::getLength() const
{
	return Math::Sqrt(getSquaredLength());
}

template<typename T>
inline Quaternion<T>& Quaternion<T>::normalize(T* oldLength)
{
	const T currentLength = getLength();
	if (oldLength != nullptr)
	{
		*oldLength = currentLength;
	}
	const T factor = T(1) / currentLength;
	v *= factor;
	s *= factor;
	return *this;
}

template<typename T>
inline Quaternion<T> Quaternion<T>::normalized(T* oldLength)
{
	return Quaternion<T>(*this).normalize(oldLength);
}

template<typename T>
inline Quaternion<T>& Quaternion<T>::ensureNormalized()
{
	if (!Math::Equals(getSquaredLength(), T(1)))
	{
		return normalize();
	}
	return *this;
}

template<typename T>
inline void Quaternion<T>::toEulerAngles(Vector3<T>& vector) const
{
	const Matrix3<T> matrix(toMatrix3());
	const T cos2 = matrix[0] * matrix[0] + matrix[1] * matrix[1];
	if (cos2 < T(0.000001))
	{
		vector.set(0, (matrix[2] < 0) ? Math::HALF_PI : -Math::HALF_PI, -Math::Atan2(matrix[4], matrix[3]));
	}
	else
	{
		vector.set(Math::Atan2(matrix[8], matrix[5]), Math::Atan2(Math::Sqrt(cos2), -matrix[2]), Math::Atan2(matrix[0], matrix[1]));
	}
}

template<typename T>
inline Vector3<T> Quaternion<T>::toEulerAngles() const
{
	const Matrix3<T> matrix(toMatrix3());
	const T cos2 = matrix[0] * matrix[0] + matrix[1] * matrix[1];
	if (cos2 < T(0.000001))
	{
		return Vector3<T>(0, (matrix[2] < 0) ? Math::HALF_PI : -Math::HALF_PI, -Math::Atan2(matrix[4], matrix[3]));
	}
	else
	{
		return Vector3<T>(Math::Atan2(matrix[8], matrix[5]), Math::Atan2(Math::Sqrt(cos2), -matrix[2]), Math::Atan2(matrix[0], matrix[1]));
	}
}

template<typename T>
inline void Quaternion<T>::toAngleAxis(T& angle, Vector3<T>& axis) const
{
	axis = (s > T(0)) ? v : -v;
	angle = T(2) * Math::Atan(Math::Abs(s));
}

template<typename T>
inline void Quaternion<T>::toMatrix3(Matrix3<T>& matrix) const
{
	const T x2 = v.x * v.x; const T y2 = v.y * v.y; const T z2 = v.z * v.z;
	const T xs = v.x * s;   const T ys = v.y * s;   const T zs = v.z * s;
	const T xy = v.x * v.y; const T xz = v.x * v.z; const T yz = v.y * v.z;
	matrix.set(1 - 2 * (y2 + z2), 2 * (xy + zs), 2 * (xz - ys),
		2 * (xy - zs), 1 - 2 * (x2 + z2), 2 * (xs + yz),
		2 * (ys + xz), 2 * (yz - xs), 1 - 2 * (x2 + y2));
}

template<typename T>
inline Matrix3<T> Quaternion<T>::toMatrix3() const
{
	const T x2 = v.x * v.x; const T y2 = v.y * v.y; const T z2 = v.z * v.z;
	const T xs = v.x * s;   const T ys = v.y * s;   const T zs = v.z * s;
	const T xy = v.x * v.y; const T xz = v.x * v.z; const T yz = v.y * v.z;
	return Matrix3<T>(1 - 2 * (y2 + z2), 2 * (xy + zs), 2 * (xz - ys),
		2 * (xy - zs), 1 - 2 * (x2 + z2), 2 * (xs + yz),
		2 * (ys + xz), 2 * (yz - xs), 1 - 2 * (x2 + y2));
}

template<typename T>
inline void Quaternion<T>::toMatrix4(Matrix4<T>& matrix) const
{
	const T x2 = v.x * v.x; const T y2 = v.y * v.y; const T z2 = v.z * v.z;
	const T xs = v.x * s;   const T ys = v.y * s;   const T zs = v.z * s;
	const T xy = v.x * v.y; const T xz = v.x * v.z; const T yz = v.y * v.z;
	matrix.set(1 - 2 * (y2 + z2), 2 * (xy + zs), 2 * (xz - ys), 0,
		2 * (xy - zs), 1 - 2 * (x2 + z2), 2 * (xs + yz), 0,
		2 * (ys + xz), 2 * (yz - xs), 1 - 2 * (x2 + y2), 0,
		0, 0, 0, 1);
}

template<typename T>
inline Matrix4<T> Quaternion<T>::toMatrix4() const
{
	const T x2 = v.x * v.x; const T y2 = v.y * v.y; const T z2 = v.z * v.z;
	const T xs = v.x * s;   const T ys = v.y * s;   const T zs = v.z * s;
	const T xy = v.x * v.y; const T xz = v.x * v.z; const T yz = v.y * v.z;
	return Matrix4<T>(1 - 2 * (y2 + z2), 2 * (xy + zs), 2 * (xz - ys), 0,
		2 * (xy - zs), 1 - 2 * (x2 + z2), 2 * (xs + yz), 0,
		2 * (ys + xz), 2 * (yz - xs), 1 - 2 * (x2 + y2), 0,
		0, 0, 0, 1);
}

template <typename T>
inline Vector4<T> Quaternion<T>::toVector4() const
{
	return Vector4<T>(v.x, v.y, v.z, s);
}

template<typename T>
inline Quaternion<T>& Quaternion<T>::fromEulerAngles(const Vector3<T>& vector)
{
	Vector3<T> halfAngles(T(0.5) * vector);
	const T sx = Math::Sin(halfAngles.x);
	const T cx = Math::Cos(halfAngles.x);
	const T sy = Math::Sin(halfAngles.y);
	const T cy = Math::Cos(halfAngles.y);
	const T sz = Math::Sin(halfAngles.z);
	const T cz = Math::Cos(halfAngles.z);
	v.x = sx * cy * cz - cx * sy * sz;
	v.y = cx * sy * cz + sx * cy * sz;
	v.z = cx * cy * sz - sx * sy * cz;
	s = cx * cy * cz + sx * sy * sz;
	return *this;
}

template<typename T>
inline Quaternion<T>& Quaternion<T>::fromAngleAxis(const T& angle, const Vector3<T>& axis)
{
	const T halfAngle = angle * T(0.5);
	v.set(axis.normalized() * Math::Sin(halfAngle));
	s = Math::Cos(halfAngle);
	return *this;
}

template<typename T>
inline Quaternion<T>& Quaternion<T>::fromMatrix3(const Matrix3<T>& matrix)
{
	const T trace = matrix.getTrace();
	if (trace > 0)
	{
		const T t = Math::Sqrt(trace + 1) * 2;
		const T overT = 1 / t;
		v.x = (matrix[5] - matrix[7]) * overT;
		v.y = (matrix[6] - matrix[2]) * overT;
		v.z = (matrix[1] - matrix[3]) * overT;
		s = t * T(0.25);
		return *this;
	}
	else if (matrix[0] > matrix[4] && matrix[0] > matrix[8])
	{
		const T t = Math::Sqrt(matrix[0] - matrix[4] - matrix[8] + 1) * 2;
		const T overT = 1 / t;
		v.x = t * T(0.25);
		v.y = (matrix[3] + matrix[1]) * overT;
		v.z = (matrix[6] + matrix[2]) * overT;
		s = (matrix[5] - matrix[7]) * overT;
		return *this;
	}
	else if (matrix[4] > matrix[8])
	{
		const T t = Math::Sqrt(matrix[4] - matrix[0] - matrix[8] + 1) * 2;
		const T overT = 1 / t;
		v.x = (matrix[3] + matrix[1]) * overT;
		v.y = t * T(0.25);
		v.z = (matrix[5] + matrix[7]) * overT;
		s = (matrix[6] - matrix[2]) * overT;
		return *this;
	}
	else
	{
		const T t = Math::Sqrt(matrix[8] - matrix[0] - matrix[4] + 1) * 2;
		const T overT = 1 / t;
		v.x = (matrix[6] + matrix[2]) * overT;
		v.y = (matrix[5] + matrix[7]) * overT;
		v.z = t * T(0.25);
		s = (matrix[1] - matrix[3]) * overT;
		return *this;
	}
	return *this;
}

template<typename T>
inline Quaternion<T>& Quaternion<T>::fromVector4(const Vector4<T>& vector)
{
	v.x = vector.x;
	v.y = vector.y;
	v.z = vector.z;
	s = vector.w;
	return *this;
}

template<typename T>
inline Quaternion<T> Quaternion<T>::slerp(const Quaternion<T>& start, const Quaternion<T>& end, const T& percent)
{
	if (start.dotProduct(end) > T(0.999999f))
	{
		T mp = 1 - percent;
		return Quaternion<T>(start.v * mp + end.v * percent, start.s * mp + end.s * percent);
	}
	return Quaternion<T>(start * ((start.inversed() * end) * percent));
}

template<typename T>
inline Quaternion<T> Quaternion<T>::rotateFromTo(const Vector3<T>& start, const Vector3<T>& end)
{
	Vector3<T> v1 = start.normalized();
	Vector3<T> v2 = end.normalized();
	const T dot = v1.dotProduct(v2);
	if (dot >= T(0.99999847691))
	{
		return Quaternion<T>::identity;
	}
	if (dot <= T(-0.99999847691))
	{
		return Quaternion<T>(0, v1.perpendicularVector());
	}
	Vector3<T> cross = v1.crossProduct(v2);
	return Quaternion<T>(cross, dot + T(1)).normalize();
}

template<typename T>
inline Quaternion<T> Quaternion<T>::rotateFromTo(const Vector3<T>& start, const Vector3<T>& end, const Vector3<T>& axis)
{
	Vector3<T> v1 = start.normalized();
	Vector3<T> v2 = end.normalized();
	const T dot = v1.dotProduct(v2);
	if (dot >= T(0.99999847691))
	{
		return Quaternion<T>::identity;
	}
	if (dot <= T(-0.99999847691))
	{
		return Quaternion<T>(0, axis);
	}
	Vector3<T> cross = v1.crossProduct(v2);
	return Quaternion<T>(cross, dot + T(1)).normalize();
}

template<typename T>
inline Quaternion<T>& Quaternion<T>::makeIdentity()
{
	v.makeZero();
	s = T(1);
}

template<typename T>
inline Quaternion<T> operator*(const T& s, const Quaternion<T>& quaternion)
{
	return quaternion * s;
}

template <typename T> const Quaternion<T> Quaternion<T>::identity(Vector3<T>::zero, 1);

typedef Quaternion<F32> Quaternionf;

typedef Quaternionf quat; // GLSL-like

} // namespace en

ENLIVE_DEFINE_TYPE_INFO_TEMPLATE(en::Quaternion)
