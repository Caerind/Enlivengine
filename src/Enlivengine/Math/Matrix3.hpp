#pragma once

#include <Enlivengine/Math/Vector3.hpp>

#include <glm/gtc/matrix_access.hpp>
#include <glm/gtc/matrix_inverse.hpp>
#include <glm/gtx/matrix_query.hpp>

namespace en
{

template <typename T>
struct Matrix3 : public glm::mat<3, 3, T, glm::defaultp>
{
public:
	using Parent = glm::mat<3, 3, T, glm::defaultp>;
	using ElementType = T;
	static constexpr glm::qualifier Precision = glm::defaultp;
	using ColumnType = Vector3<T>;
	using RowType = Vector3<T>;
	using TransposeType = Matrix3<T>;
	static constexpr U32 Rows{ 3 };
	static constexpr U32 Columns{ 3 };
	static constexpr U32 Elements{ Rows * Columns };

	// Constructors
	constexpr Matrix3() : Parent() {}
	constexpr Matrix3(const Matrix3<T>& m) : Parent(static_cast<Parent>(m)) {}
	constexpr explicit Matrix3(T scalar) : Parent(scalar) {}
	constexpr Matrix3(T x0, T y0, T z0, T x1, T y1, T z1, T x2, T y2, T z2) : Parent(x0, y0, z0, x1, y1, z1, x2, y2, z2) {}
	constexpr Matrix3(const T* data) : Parent(data[0], data[1], data[2], data[3], data[4], data[5], data[6], data[7], data[8]) {}
	constexpr Matrix3(const Vector3<T>& col0, const Vector3<T>& col1, const Vector3<T>& col2) : Parent(static_cast<Vector3<T>::Parent>(col0), static_cast<Vector3<T>::Parent>(col1), static_cast<Vector3<T>::Parent>(col2)) {}
	template <typename U> 
	constexpr Matrix3(const Matrix3<U>& m) : Parent(static_cast<Matrix3<U>::Parent>(m)) {}
	constexpr Matrix3(const Parent& parent) : Parent(parent) {}
	constexpr Matrix3(const glm::mat<4, 4, T, Precision>& m) : Parent(m) {}
	~Matrix3() = default;

	// Operators
	constexpr Matrix3<T>& operator=(const Matrix3<T>& m) { Parent::operator=(static_cast<Parent>(m)); return *this; }
	constexpr Matrix3<T> operator+() const { return Matrix3(+static_cast<Parent>(*this)); }
	constexpr Matrix3<T> operator-() const { return Matrix3(-static_cast<Parent>(*this)); }
	constexpr Matrix3<T>& operator+=(const Matrix3<T>& m) { Parent::operator+=(static_cast<Parent>(m)); return *this; }
	constexpr Matrix3<T>& operator-=(const Matrix3<T>& m) { Parent::operator-=(static_cast<Parent>(m)); return *this; }
	constexpr Matrix3<T>& operator*=(const Matrix3<T>& m) { Parent::operator*=(static_cast<Parent>(m)); return *this; }
	constexpr Matrix3<T>& operator/=(const Matrix3<T>& m) { Parent::operator/=(static_cast<Parent>(m)); return *this; }
	constexpr Matrix3<T> operator+(const Matrix3<T>& m) const { return Matrix3(static_cast<Parent>(*this) + static_cast<Parent>(m)); }
	constexpr Matrix3<T> operator-(const Matrix3<T>& m) const { return Matrix3(static_cast<Parent>(*this) - static_cast<Parent>(m)); }
	constexpr Matrix3<T> operator*(const Matrix3<T>& m) const { return Matrix3(static_cast<Parent>(*this) * static_cast<Parent>(m)); }
	constexpr Matrix3<T> operator/(const Matrix3<T>& m) const { return Matrix3(static_cast<Parent>(*this) / static_cast<Parent>(m)); }
	constexpr Matrix3<T>& operator+=(T scalar) { Parent::operator+=(scalar); return *this; }
	constexpr Matrix3<T>& operator-=(T scalar) { Parent::operator-=(scalar); return *this; }
	constexpr Matrix3<T>& operator*=(T scalar) { Parent::operator*=(scalar); return *this; }
	constexpr Matrix3<T>& operator/=(T scalar) { Parent::operator/=(scalar); return *this; }
	constexpr Matrix3<T> operator+(T scalar) const { return Matrix3(static_cast<Parent>(*this) + scalar); }
	constexpr Matrix3<T> operator-(T scalar) const { return Matrix3(static_cast<Parent>(*this) - scalar); }
	constexpr Matrix3<T> operator*(T scalar) const { return Matrix3(static_cast<Parent>(*this) * scalar); }
	constexpr Matrix3<T> operator/(T scalar) const { return Matrix3(static_cast<Parent>(*this) / scalar); }
	constexpr Vector3<T> operator*(const Vector3<T>& v) const { return Vector3(static_cast<Parent>(*this) * static_cast<Vector3<T>::Parent>(v)); }
	constexpr bool operator==(const Matrix3<T>& m) const { return static_cast<Parent>(*this) == static_cast<Parent>(m); }
	constexpr bool operator!=(const Matrix3<T>& m) const { return static_cast<Parent>(*this) != static_cast<Parent>(m); }
	bool IsIdentity() const { return glm::isIdentity(static_cast<Parent>(*this), T(Math::Epsilon)); }
	bool IsOrthogonal() const { return glm::isOrthogonal(static_cast<Parent>(*this), T(Math::Epsilon)); }
	bool IsNull() const { return glm::isNull(static_cast<Parent>(*this), T(Math::Epsilon)); }
	bool IsNormalized() const { return glm::isNormalized(static_cast<Parent>(*this), T(Math::Epsilon)); }

	// Accessors
	Vector3<T> GetColumn(U32 index) const { return Vector3(glm::column(static_cast<Parent>(*this), static_cast<int>(index))); }
	Vector3<T> GetRow(U32 index) const { return Vector3(glm::row(static_cast<Parent>(*this), static_cast<int>(index))); }
	void SetColumn(U32 index, const Vector3<T>& column) { *this = Matrix3(glm::column(static_cast<Parent>(*this), static_cast<int>(index), static_cast<Vector3<T>::Parent>(column))); }
	void SetRow(U32 index, const Vector3<T>& row) { *this = Matrix3(glm::row(static_cast<Parent>(*this), static_cast<int>(index), static_cast<Vector3<T>::Parent>(row))); }
	T* GetData() { return const_cast<T*>(glm::value_ptr(static_cast<Parent>(*this))); }
	const T* GetData() const { return glm::value_ptr(static_cast<Parent>(*this)); }

	// Constants
	static constexpr Matrix3<T> Identity() { return Matrix3(T(1)); }
	static Matrix3<T> Diagonal(const Vector3<T>& v) { return Matrix3(glm::diagonal3x3(static_cast<Vector3<T>::Parent>(v))); }

	// Operations
	T GetTrace() const { return (*this)[0][0] + (*this)[1][1] + (*this)[2][2]; }
	T GetDeterminant() const { return glm::determinant(static_cast<Parent>(*this)); }
	//Matrix3<T>& AffineInverse() { *this = Matrix3(glm::affineInverse(static_cast<Parent>(*this))); return *this; }
	//Matrix3<T> AffineInversed() { return Matrix3(glm::affineInverse(static_cast<Parent>(*this))); }
	Matrix3<T>& Inverse() { *this = Matrix3(glm::inverse(static_cast<Parent>(*this))); return *this; }
	Matrix3<T> Inversed() const { return Matrix3(glm::inverse(static_cast<Parent>(*this))); }
	Matrix3<T>& Transpose() { *this = Matrix3(glm::transpose(static_cast<Parent>(*this))); return *this; }
	Matrix3<T> Transposed() const { return Matrix3(glm::transpose(static_cast<Parent>(*this))); }
	Matrix3<T>& InverseTranspose() { *this = Matrix3(glm::inverseTranspose(static_cast<Parent>(*this))); return *this; }
	Matrix3<T> InverseTransposed() { return Matrix3(glm::inverseTranspose(static_cast<Parent>(*this))); }

	// Directions
	constexpr Vector3<T> GetForward() const { return (*this) * (ENLIVE_DEFAULT_FORWARD); }
	constexpr Vector3<T> GetBackward() const { return (*this) * (ENLIVE_DEFAULT_BACKWARD); }
	constexpr Vector3<T> GetUp() const { return (*this) * (ENLIVE_DEFAULT_UP); }
	constexpr Vector3<T> GetDown() const { return (*this) * (ENLIVE_DEFAULT_DOWN); }
	constexpr Vector3<T> GetLeft() const { return (*this) * (ENLIVE_DEFAULT_LEFT); }
	constexpr Vector3<T> GetRight() const { return (*this) * (ENLIVE_DEFAULT_RIGHT); }

	// Rotations
	static Matrix3<T> RotationX(T angle) { return Rotation(angle, Vector3<T>::UnitX()); }
	static Matrix3<T> RotationY(T angle) { return Rotation(angle, Vector3<T>::UnitY()); }
	static Matrix3<T> RotationZ(T angle) { return Rotation(angle, Vector3<T>::UnitZ()); }
	static Matrix3<T> Rotation(T angle, const Vector3<T>& axis) { return Matrix3(glm::rotate(angle, static_cast<Vector3<T>::Parent>(axis))); }
	static Matrix3<T> Rotation(T angle, T axisX, T axisY, T axisZ) { return Matrix3(glm::rotate(angle, axisX, axisY, axisZ)); }

	// AngleAxis
	void ToAngleAxis(Vector3<T>& axis, T& angle) const { glm::axisAngle(glm::mat<4, 4, T, Precision>(static_cast<Parent>(*this)), static_cast<Vector3<T>::Parent>(axis), angle); }
	void FromAngleAxis(const Vector3<T>& axis, T angle) { *this = Matrix3(glm::axisAngleMatrix(static_cast<Vector3<T>::Parent>(axis), angle)); }

	// Meta
	bool Serialize(Serializer& serializer, const char* name);
	bool Edit(ObjectEditor& objectEditor, const char* name);
};

template <typename T>
bool Matrix3<T>::Serialize(Serializer& serializer, const char* name)
{
	if (serializer.BeginClass(name, TypeInfo<Matrix3<T>>::GetName(), TypeInfo<Matrix3<T>>::GetHash()))
	{
		bool ret = true;
		for (U32 row = 0; row < Matrix3<T>::Rows; ++row)
		{
			for (U32 col = 0; col < Matrix3<T>::Columns; ++col)
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
bool Matrix3<T>::Edit(ObjectEditor& objectEditor, const char* name)
{
	if (objectEditor.BeginClass(name, TypeInfo<Matrix3<T>>::GetName(), TypeInfo<Matrix3<T>>::GetHash()))
	{
		bool ret = false;
		for (U32 row = 0; row < Matrix3<T>::Rows; ++row)
		{
			const std::string childName("Row_" + std::to_string(row));
			Vector3<T> v = GetRow(row);
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

typedef Matrix3<F32> Matrix3f;

typedef Matrix3f mat3; // GLSL-like

} // namespace en

ENLIVE_DEFINE_TYPE_INFO_TEMPLATE(en::Matrix3, en::Type_CustomSerialization, en::Type_CustomEditor)
