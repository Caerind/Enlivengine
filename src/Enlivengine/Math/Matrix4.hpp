#pragma once

#include <Enlivengine/Math/Vector4.hpp>
#include <Enlivengine/Math/Matrix3.hpp>
#include <Enlivengine/Math/Quaternion.hpp>

namespace en
{

template <typename T>
struct Matrix4 : public glm::mat<4, 4, T, glm::defaultp>
{
public:
	using Parent = glm::mat<4, 4, T, glm::defaultp>;
	using ElementType = T;
	static constexpr glm::qualifier Precision = glm::defaultp;
	using ColumnType = Vector4<T>;
	using RowType = Vector4<T>;
	using TransposeType = Matrix4<T>;
	static constexpr U32 Rows{ 4 };
	static constexpr U32 Columns{ 4 };
	static constexpr U32 Elements{ Rows * Columns };

	// Constructors
	constexpr Matrix4() : Parent(glm::identity<Parent>()) {}
	constexpr Matrix4(const Matrix4<T>& m) : Parent(static_cast<Parent>(m)) {}
	constexpr explicit Matrix4(T scalar) : Parent(scalar) {}
	constexpr Matrix4(T x0, T y0, T z0, T w0, T x1, T y1, T z1, T w1, T x2, T y2, T z2, T w2, T x3, T y3, T z3, T w3) : Parent(x0, y0, z0, w0, x1, y1, z1, w1, x2, y2, z2, w2, x3, y3, z3, w3) {}
	constexpr Matrix4(const T* data) : Parent(data[0], data[1], data[2], data[3], data[4], data[5], data[6], data[7], data[8], data[9], data[10], data[11], data[12], data[13], data[14], data[15]) {}
	constexpr Matrix4(const Vector4<T>& col0, const Vector4<T>& col1, const Vector3<T>& col2, const Vector3<T>& col3) : Parent(static_cast<Vector3<T>::Parent>(col0), static_cast<Vector3<T>::Parent>(col1), static_cast<Vector3<T>::Parent>(col2), static_cast<Vector3<T>::Parent>(col3)) {}
	template <typename U>
	constexpr Matrix4(const Matrix4<U>& m) : Parent(static_cast<Matrix4<U>::Parent>(m)) {}
	constexpr Matrix4(const Parent& parent) : Parent(parent) {}
	constexpr Matrix4(const Matrix3<T>& m) : Parent(static_cast<Matrix3<T>::Parent>(m)) {}
	~Matrix4() = default;

	// Operators
	constexpr Matrix4<T>& operator=(const Matrix4<T>& m) { Parent::operator=(static_cast<Parent>(m)); return *this; }
	constexpr Matrix4<T> operator+() const { return Matrix4(+static_cast<Parent>(*this)); }
	constexpr Matrix4<T> operator-() const { return Matrix4(-static_cast<Parent>(*this)); }
	constexpr Matrix4<T>& operator+=(const Matrix4<T>& m) { Parent::operator+=(static_cast<Parent>(m)); return *this; }
	constexpr Matrix4<T>& operator-=(const Matrix4<T>& m) { Parent::operator-=(static_cast<Parent>(m)); return *this; }
	constexpr Matrix4<T>& operator*=(const Matrix4<T>& m) { Parent::operator*=(static_cast<Parent>(m)); return *this; }
	constexpr Matrix4<T>& operator/=(const Matrix4<T>& m) { Parent::operator/=(static_cast<Parent>(m)); return *this; }
	constexpr Matrix4<T> operator+(const Matrix4<T>& m) const { return Matrix4(static_cast<Parent>(*this) + static_cast<Parent>(m)); }
	constexpr Matrix4<T> operator-(const Matrix4<T>& m) const { return Matrix4(static_cast<Parent>(*this) - static_cast<Parent>(m)); }
	constexpr Matrix4<T> operator*(const Matrix4<T>& m) const { return Matrix4(static_cast<Parent>(*this) * static_cast<Parent>(m)); }
	constexpr Matrix4<T> operator/(const Matrix4<T>& m) const { return Matrix4(static_cast<Parent>(*this) / static_cast<Parent>(m)); }
	constexpr Matrix4<T>& operator+=(T scalar) { Parent::operator+=(scalar); return *this; }
	constexpr Matrix4<T>& operator-=(T scalar) { Parent::operator-=(scalar); return *this; }
	constexpr Matrix4<T>& operator*=(T scalar) { Parent::operator*=(scalar); return *this; }
	constexpr Matrix4<T>& operator/=(T scalar) { Parent::operator/=(scalar); return *this; }
	constexpr Matrix4<T> operator+(T scalar) const { return Matrix4(static_cast<Parent>(*this) + scalar); }
	constexpr Matrix4<T> operator-(T scalar) const { return Matrix4(static_cast<Parent>(*this) - scalar); }
	constexpr Matrix4<T> operator*(T scalar) const { return Matrix4(static_cast<Parent>(*this) * scalar); }
	constexpr Matrix4<T> operator/(T scalar) const { return Matrix4(static_cast<Parent>(*this) / scalar); }
	constexpr Vector4<T> operator*(const Vector4<T>& v) const { return Vector4(static_cast<Parent>(*this) * static_cast<Vector4<T>::Parent>(v)); }
	constexpr bool operator==(const Matrix4<T>& m) const { return static_cast<Parent>(*this) == static_cast<Parent>(m); }
	constexpr bool operator!=(const Matrix4<T>& m) const { return static_cast<Parent>(*this) != static_cast<Parent>(m); }
	bool IsIdentity() const { return glm::isIdentity(static_cast<Parent>(*this), T(Math::Epsilon)); }
	bool IsOrthogonal() const { return glm::isOrthogonal(static_cast<Parent>(*this), T(Math::Epsilon)); }
	bool IsNull() const { return glm::isNull(static_cast<Parent>(*this), T(Math::Epsilon)); }
	bool IsNormalized() const { return glm::isNormalized(static_cast<Parent>(*this), T(Math::Epsilon)); }
	static bool Equals(const Matrix4<T>& m1, const Matrix4<T>& m2, T epsilon = T(Math::Epsilon)) { return Vector4<T>::Equals(m1[0], m2[0], epsilon) && Vector4<T>::Equals(m1[1], m2[1], epsilon) && Vector4<T>::Equals(m1[2], m2[2], epsilon) && Vector4<T>::Equals(m1[3], m2[3], epsilon); }

	// Accessors
	Vector4<T> GetColumn(U32 index) const { return Vector4(glm::column(static_cast<Parent>(*this), static_cast<int>(index))); }
	Vector4<T> GetRow(U32 index) const { return Vector4(glm::row(static_cast<Parent>(*this), static_cast<int>(index))); }
	void SetColumn(U32 index, const Vector4<T>& column) { *this = Matrix4(glm::column(static_cast<Parent>(*this), static_cast<int>(index), static_cast<Vector4<T>::Parent>(column))); }
	void SetRow(U32 index, const Vector4<T>& row) { *this = Matrix4(glm::row(static_cast<Parent>(*this), static_cast<int>(index), static_cast<Vector4<T>::Parent>(row))); }
	T* GetData() { return const_cast<T*>(glm::value_ptr(static_cast<Parent>(*this))); }
	const T* GetData() const { return glm::value_ptr(static_cast<Parent>(*this)); }

	// Constants
	static constexpr Matrix4<T> Identity() { return Matrix4(glm::identity<Parent>()); }
	static Matrix4<T> Diagonal(const Vector4<T>& v) { return Matrix4(glm::diagonal4x4(static_cast<Vector4<T>::Parent>(v))); }

	// Operations
	Vector3<T> TransformDirection(const Vector3<T>& direction) const { return ((*this) * Vector4(direction, 0.0f)).xyz(); }
	Vector3<T> TransformPoint(const Vector3<T>& point) const { return ((*this) * Vector4(point, 1.0f)).xyz(); }
	T GetTrace() const { return (*this)[0][0] + (*this)[1][1] + (*this)[2][2] + (*this)[3][3]; }
	T GetDeterminant() const { return glm::determinant(static_cast<Parent>(*this)); }
	Matrix4<T>& AffineInverse() { *this = Matrix4(glm::affineInverse(static_cast<Parent>(*this))); return *this; }
	Matrix4<T> AffineInversed() { return Matrix4(glm::affineInverse(static_cast<Parent>(*this))); }
	Matrix4<T>& Inverse() { *this = Matrix4(glm::inverse(static_cast<Parent>(*this))); return *this; }
	Matrix4<T> Inversed() const { return Matrix4(glm::inverse(static_cast<Parent>(*this))); }
	Matrix4<T>& Transpose() { *this = Matrix4(glm::transpose(static_cast<Parent>(*this))); return *this; }
	Matrix4<T> Transposed() const { return Matrix4(glm::transpose(static_cast<Parent>(*this))); }
	Matrix4<T>& InverseTranspose() { *this = Matrix4(glm::inverseTranspose(static_cast<Parent>(*this))); return *this; }
	Matrix4<T> InverseTransposed() { return Matrix4(glm::inverseTranspose(static_cast<Parent>(*this))); }

	// Directions
	constexpr Vector3<T> GetForward() const { return TransformDirection(ENLIVE_DEFAULT_FORWARD); }
	constexpr Vector3<T> GetBackward() const { return TransformDirection(ENLIVE_DEFAULT_BACKWARD); }
	constexpr Vector3<T> GetUp() const { return TransformDirection(ENLIVE_DEFAULT_UP); }
	constexpr Vector3<T> GetDown() const { return TransformDirection(ENLIVE_DEFAULT_DOWN); }
	constexpr Vector3<T> GetLeft() const { return TransformDirection(ENLIVE_DEFAULT_LEFT); }
	constexpr Vector3<T> GetRight() const { return TransformDirection(ENLIVE_DEFAULT_RIGHT); }

	// Transform ctors
	static Matrix4<T> Rotation(const Matrix3<T>& m) { return Matrix4(m); }
	// TODO : GLM update
	//static Matrix4<T> Rotation(const Quaternion<T>& q) { return Matrix4(glm::mat4_cast(static_cast<Quaternion<T>::Parent>(q))); }
	static Matrix4<T> RotationX(T angle) { return RotationAxis(angle, Vector3<T>::UnitX()); }
	static Matrix4<T> RotationY(T angle) { return RotationAxis(angle, Vector3<T>::UnitY()); }
	static Matrix4<T> RotationZ(T angle) { return RotationAxis(angle, Vector3<T>::UnitZ()); }
	static Matrix4<T> RotationAxis(T angle, const Vector3<T>& axis) { return Matrix4(glm::rotate(angle * T(Math::kDegToRad), static_cast<Vector3<T>::Parent>(axis))); }
	static Matrix4<T> RotationAxis(T angle, T axisX, T axisY, T axisZ) { return RotationAxis(angle, Vector3<T>(axisX, axisY, axisZ)); }
	static Matrix4<T> Scale(const Vector3<T>& scale) { return Matrix4(glm::scale(static_cast<Vector3<T>::Parent>(scale))); }
	static Matrix4<T> Scale(T sx, T sy, T sz) { return Scale(Vector3<T>(sx, sy, sz)); }
	static Matrix4<T> Scale(T s) { return Scale(Vector3<T>(s)); }
	static Matrix4<T> Translation(const Vector3<T>& translation) { return Matrix4(glm::translate(static_cast<Vector3<T>::Parent>(translation))); }
	static Matrix4<T> Translation(T tx, T ty, T tz) { return Translation(Vector3<T>(tx, ty, tz)); }
	static Matrix4<T> Transform(const Vector3<T>& translation, const Matrix3<T>& rotation, const Vector3<T>& scale = Vector3<T>(1)) { return Matrix4(glm::translate(static_cast<Vector3<T>::Parent>(translation)) * glm::mat<4, 4, T, Precision>(static_cast<Matrix3<T>::Parent>(rotation)) * glm::scale(static_cast<Vector3<T>::Parent>(scale))); }
	// TODO : GLM update
	//static Matrix4<T> Transform(const Vector3<T>& translation, const Quaternion<T>& rotation, const Vector3<T>& scale = Vector3<T>(1)) { return Matrix4(glm::translate(static_cast<Vector3<T>::Parent>(translation)) * glm::mat4_cast(static_cast<Quaternion<T>::Parent>(rotation)) * glm::scale(static_cast<Vector3<T>::Parent>(scale))); }

	// Translation
	Vector3<T> GetTranslation() const { return Vector4((*this)[3]).xyz(); }
	void SetTranslation(const Vector3<T>& translation) { (*this)[3] = glm::vec<4, T, Precision>(static_cast<Vector3<T>::Parent>(translation), T(1)); }
	void SetTranslation(T tx, T ty, T tz) { SetTranslation(Vector3<T>(tx, ty, tz)); }
	void ApplyTranslation(const Vector3<T>& translation) { *this = Matrix4(glm::translate(static_cast<Parent>(*this), static_cast<Vector3<T>::Parent>(translation))); }
	void ApplyTranslation(T tx, T ty, T tz) { ApplyTranslation(Vector3<T>(tx, ty, tz)); }

	// Scale
	bool IsScaled() const { return !Vector3<T>::Equals(GetScale(), Vector3<T>::Unit()); }
	bool HasUniformScale() const { const Vector3<T> scale = GetScale(); return Math::Equals(scale.x, scale.y) && Math::Equals(scale.x, scale.z); }
	Vector3<T> GetScale() const { return Vector3<T>(glm::length(glm::vec3((*this)[0])), glm::length(glm::vec3((*this)[1])), glm::length(glm::vec3((*this)[2]))); }
	void SetScale(const Vector3<T>& scale) { const Vector3<T> scaleChange = scale / GetScale(); (*this)[0] *= scaleChange.x; (*this)[1] *= scaleChange.y; (*this)[2] *= scaleChange.z; } // TODO : Check
	void SetScale(T sx, T sy, T sz) { SetScale(Vector3<T>(sx, sy, sz)); }
	void SetScale(T s) { SetScale(Vector3<T>(s)); }
	void ApplyScale(const Vector3<T>& scale) { *this = Matrix4(glm::scale(static_cast<Parent>(*this), static_cast<Vector3<T>::Parent>(scale))); }
	void ApplyScale(T sx, T sy, T sz) { ApplyScale(Vector3<T>(sx, sy, sz)); }
	void ApplyScale(T s) { ApplyScale(Vector3<T>(s)); }

	// Rotation
	Matrix3<T> GetRotation() const { const Vector3<T> scale = GetScale(); return Matrix3<T>(Vector3(glm::vec<3, T, Precision>((*this)[0]) / scale.x), Vector3(glm::vec<3, T, Precision>((*this)[1]) / scale.y), Vector3(glm::vec<3, T, Precision>((*this)[2]) / scale.z)); }
	void SetRotation(const Matrix3<T>& rotation) { const Vector3<T> scale = GetScale(); (*this)[0] = scale.x * Vector4f(rotation.GetColumn(0), 0.0f); (*this)[1] = scale.y * Vector4f(rotation.GetColumn(1), 0.0f); (*this)[2] = scale.z * Vector4f(rotation.GetColumn(2), 0.0f); }
	void ApplyRotation(const Matrix3<T>& rotation) { *this *= Matrix4(rotation); }

	// TODO : GLM update
	//Quaternion<T> GetQuaternion() const { return Quaternion<T>(glm::quat_cast(static_cast<Parent>(*this))); }
	//void ApplyQuaternion(const Quaternion<T>& quaternion) { ApplyRotation(Rotation(quaternion)); }

	// View/Projection
	static Matrix4<T> Perspective(T fov, T aspect, T nearPlane, T farPlane) { return Matrix4(glm::perspective(fov * T(Math::kDegToRad), aspect, nearPlane, farPlane)); }
	static Matrix4<T> Perspective(T fov, T width, T height, T nearPlane, T farPlane) { return Matrix4(glm::perspectiveFov(fov * T(Math::kDegToRad), width, height, nearPlane, farPlane)); }
	static Matrix4<T> InfinitePerspective(T fov, T aspect, T nearPlane) { return Matrix4(glm::infinitePerspective(fov * T(Math::kDegToRad), aspect, nearPlane)); }
	static Matrix4<T> Orthographic(T left, T top, T right, T bottom) { return Matrix4(glm::ortho(left, top, right, bottom)); }
	static Matrix4<T> Orthographic(T left, T top, T right, T bottom, T nearPlane, T farPlane) { return Matrix4(glm::ortho(left, top, right, bottom, nearPlane, farPlane)); }
	static Matrix4<T> LookAt(const Vector3<T>& eye, const Vector3<T>& target, const Vector3<T>& up) { return Matrix4(glm::lookAt(static_cast<Vector3<T>::Parent>(eye), static_cast<Vector3<T>::Parent>(target), static_cast<Vector3<T>::Parent>(up))); }

	// Meta
	bool Serialize(Serializer& serializer, const char* name);
	bool Edit(ObjectEditor& objectEditor, const char* name);

protected:
	typename Parent::col_type& operator[](typename Parent::length_type i) { return Parent::operator[](i); }
	typename Parent::col_type const& operator[](typename Parent::length_type i) const { return Parent::operator[](i); }
};

template <typename T>
bool Matrix4<T>::Serialize(Serializer& serializer, const char* name)
{
	if (serializer.BeginClass(name, TypeInfo<Matrix4<T>>::GetName(), TypeInfo<Matrix4<T>>::GetHash()))
	{
		bool ret = true;
		for (U32 row = 0; row < Matrix4<T>::Rows; ++row)
		{
			for (U32 col = 0; col < Matrix4<T>::Columns; ++col)
			{
				const std::string childName("i" + std::to_string(row * Matrix3<T>::Rows + col));
				ret = GenericSerialization(serializer, childName.c_str(), (*this)[col][row]) && ret;
			}
		}
		ret = serializer.EndClass() && ret;
		return ret;
	}
	else
	{
		return false;
	}
}

template <typename T>
bool Matrix4<T>::Edit(ObjectEditor& objectEditor, const char* name)
{
	if (objectEditor.BeginClass(name, TypeInfo<Matrix4<T>>::GetName(), TypeInfo<Matrix4<T>>::GetHash()))
	{
		bool ret = false;
		for (U32 row = 0; row < Matrix4<T>::Rows; ++row)
		{
			const std::string childName("Row_" + std::to_string(row));
			Vector4<T> v = GetRow(row);
			if (GenericEdit(objectEditor, childName.c_str(), v))
			{
				SetRow(row, v);
				ret = true;
			}
		}
		objectEditor.EndClass();
		return ret;
	}
	else
	{
		return false;
	}
}

/*
template <typename T>
std::string ToString<Matrix4<T>>(const Matrix4<T>& v)
{
	return glm::to_string(static_cast<Matrix4<T>::Parent>(v));
}
*/

typedef Matrix4<F32> Matrix4f;

typedef Matrix4f mat4; // GLSL-like

} // namespace en

ENLIVE_DEFINE_TYPE_INFO_TEMPLATE(en::Matrix4, en::Type_CustomSerialization, en::Type_CustomEditor)