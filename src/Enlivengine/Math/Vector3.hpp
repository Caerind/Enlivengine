#pragma once

#include <Enlivengine/Math/Vector2.hpp>

namespace en
{

template <typename T>
struct Vector3 : public glm::vec<3, T, glm::defaultp>
{
public:
	using Parent = glm::vec<3, T, glm::defaultp>;
	using ElementType = T;
	static constexpr glm::qualifier Precision = glm::defaultp;
	static constexpr U32 Dim = 3;

	// Constructors
	constexpr Vector3() : Parent() {}
	constexpr Vector3(const Vector3<T>& v) : Parent(static_cast<Parent>(v)) {}
	constexpr explicit Vector3(T scalar) : Parent(scalar) {}
	constexpr Vector3(T x, T y, T z) : Parent(x, y, z) {}
	constexpr Vector3(const T* data) : Parent(data[0], data[1], data[2]) {}
	template <typename Other>
	constexpr Vector3(const Vector3<Other>& v) : Parent(static_cast<Vector3<Other>::Parent>(v)) {}
	template <typename OtherA, typename OtherB, typename OtherC>
	constexpr Vector3(OtherA x, OtherB y, OtherC z) : Parent(x, y, z) {}
	constexpr Vector3(const Parent& parent) : Parent(parent) {}
	constexpr Vector3(const Vector2<T>& v, T z) : Parent(static_cast<Vector2<T>::Parent>(v), z) {}
	~Vector3() = default;

	// Operators
	constexpr Vector3<T>& operator=(const Vector3<T>& v) { Parent::operator=(static_cast<Parent>(v)); return *this; }
	constexpr Vector3<T> operator+() const { return Vector3(+static_cast<Parent>(*this)); }
	constexpr Vector3<T> operator-() const { return Vector3(-static_cast<Parent>(*this)); }
	constexpr Vector3<T>& operator+=(const Vector3<T>& v) { Parent::operator+=(static_cast<Parent>(v)); return *this; }
	constexpr Vector3<T>& operator-=(const Vector3<T>& v) { Parent::operator-=(static_cast<Parent>(v)); return *this; }
	constexpr Vector3<T>& operator*=(const Vector3<T>& v) { Parent::operator*=(static_cast<Parent>(v)); return *this; }
	constexpr Vector3<T>& operator/=(const Vector3<T>& v) { Parent::operator/=(static_cast<Parent>(v)); return *this; }
	constexpr Vector3<T> operator+(const Vector3<T>& v) const { return Vector3(static_cast<Parent>(*this) + static_cast<Parent>(v)); }
	constexpr Vector3<T> operator-(const Vector3<T>& v) const { return Vector3(static_cast<Parent>(*this) - static_cast<Parent>(v)); }
	constexpr Vector3<T> operator*(const Vector3<T>& v) const { return Vector3(static_cast<Parent>(*this) * static_cast<Parent>(v)); }
	constexpr Vector3<T> operator/(const Vector3<T>& v) const { return Vector3(static_cast<Parent>(*this) / static_cast<Parent>(v)); }
	constexpr Vector3<T>& operator+=(T scalar) { Parent::operator+=(scalar); return *this; }
	constexpr Vector3<T>& operator-=(T scalar) { Parent::operator-=(scalar); return *this; }
	constexpr Vector3<T>& operator*=(T scalar) { Parent::operator*=(scalar); return *this; }
	constexpr Vector3<T>& operator/=(T scalar) { Parent::operator/=(scalar); return *this; }
	constexpr Vector3<T> operator+(T scalar) const { return Vector3(static_cast<Parent>(*this) + scalar); }
	constexpr Vector3<T> operator-(T scalar) const { return Vector3(static_cast<Parent>(*this) - scalar); }
	constexpr Vector3<T> operator*(T scalar) const { return Vector3(static_cast<Parent>(*this) * scalar); }
	constexpr Vector3<T> operator/(T scalar) const { return Vector3(static_cast<Parent>(*this) / scalar); }
	constexpr bool operator==(const Vector3<T>& v) const { return static_cast<Parent>(*this) == static_cast<Parent>(v); }
	constexpr bool operator!=(const Vector3<T>& v) const { return static_cast<Parent>(*this) != static_cast<Parent>(v); }
	constexpr bool operator<(const Vector3<T>& v) const { return x < v.x && y < v.y && z < v.z; }
	constexpr bool operator<=(const Vector3<T>& v) const { return x <= v.x && y <= v.y && z <= v.z; }
	constexpr bool operator>(const Vector3<T>& v) const { return !operator<=(v); }
	constexpr bool operator>=(const Vector3<T>& v) const { return !operator<(v); }
	bool IsZero() const { return glm::isNull(static_cast<Parent>(*this), T(Math::Epsilon)); }
	static bool Equals(const Vector3<T>& v1, const Vector3<T>& v2, T epsilon = T(Math::Epsilon)) { return Math::Equals(v1.x, v2.x, epsilon) && Math::Equals(v1.y, v2.y, epsilon) && Math::Equals(v1.z, v2.z, epsilon); }

	// Accessors
	T& operator()(U32 i) { if (i == 0) return x; else if (i == 1) return y; return z; }
	const T& operator()(U32 i) const { if (i == 0) return x; else if (i == 1) return y; return z; }
	T& operator[](U32 i) { if (i == 0) return x; else if (i == 1) return y; return z; }
	const T& operator[](U32 i) const { if (i == 0) return x; else if (i == 1) return y; return z; }
	T* GetData() { return const_cast<T*>(glm::value_ptr(static_cast<Parent>(*this))); }
	const T* GetData() const { return glm::value_ptr(static_cast<Parent>(*this)); }

	// Constants
	static constexpr Vector3<T> Unit() { return Vector3<T>(1, 1, 1); }
	static constexpr Vector3<T> UnitX() { return Vector3<T>(1, 0, 0); }
	static constexpr Vector3<T> UnitY() { return Vector3<T>(0, 1, 0); }
	static constexpr Vector3<T> UnitZ() { return Vector3<T>(0, 0, 1); }
	static constexpr Vector3<T> Zero() { return Vector3<T>(0, 0, 0); }

	// Setters
	void Set(const Vector3<T>& v) { Parent::operator=(static_cast<Parent>(v)); }
	template <typename Other>
	void Set(const Vector3<Other>& v) { Parent::operator=(static_cast<Vector3<Other>::Parent>(v)); }
	void Set(T scalar) { x = scalar; y = scalar; z = scalar; }
	void Set(T _x, T _y, T _z) { x = _x; y = _y; z = _z; }
	void Set(const T* data) { x = data[0]; y = data[1]; z = data[2]; }
	template <typename OtherA, typename OtherB, typename OtherC>
	void Set(OtherA _x, OtherB _y, OtherC _z) { x = static_cast<T>(_x); y = static_cast<T>(_y); static_cast<T>(_z); }
	void Set(const Vector2<T>& v, T _z) { x = v.x; y = v.y; z = _z; }

	// Norm
	T GetSquaredLength() const { return glm::length2(static_cast<Parent>(*this)); }
	T GetLength() const { return glm::length(static_cast<Parent>(*this)); }
	bool IsNormalized() const { return glm::isNormalized(static_cast<Parent>(*this), T(Math::Epsilon)); }
	Vector3<T>& FastNormalize() { *this = Vector3(glm::fastNormalize(static_cast<Parent>(*this))); return *this; }
	Vector3<T> FastNormalized() const { return Vector3(glm::fastNormalize(static_cast<Parent>(*this))); }
	Vector3<T>& Normalize() { *this = Vector3(glm::normalize(static_cast<Parent>(*this))); return *this; }
	Vector3<T> Normalized() const { return Vector3(glm::normalize(static_cast<Parent>(*this))); }

	// Operations
	constexpr T Dot(const Vector3<T>& v) const { return glm::dot(static_cast<Parent>(*this), static_cast<Parent>(v)); }
	static constexpr T Dot(const Vector3<T>& v1, const Vector3<T>& v2) { return glm::dot(static_cast<Parent>(v1), static_cast<Parent>(v2)); }
	T FastNormalizeDot(const Vector3<T>& v) const { return glm::fastNormalizeDot(static_cast<Parent>(*this), static_cast<Parent>(v)); }
	static T FastNormalizeDot(const Vector3<T>& v1, const Vector3<T>& v2) { return glm::fastNormalizeDot(static_cast<Parent>(v1), static_cast<Parent>(v2)); }
	T NormalizeDot(const Vector3<T>& v) const { return glm::normalizeDot(static_cast<Parent>(*this), static_cast<Parent>(v)); }
	static T NormalizeDot(const Vector3<T>& v1, const Vector3<T>& v2) { return glm::normalizeDot(static_cast<Parent>(v1), static_cast<Parent>(v2)); }
	constexpr Vector3<T> Cross(const Vector3<T>& v) const { return Vector3(glm::cross(static_cast<Parent>(*this), static_cast<Parent>(v))); }
	static constexpr Vector3<T> Cross(const Vector3<T>& v1, const Vector3<T>& v2) { return Vector3(glm::cross(static_cast<Parent>(v1), static_cast<Parent>(v2))); }
	static bool AreCollinear(const Vector3<T>& v1, const Vector3<T>& v2) { return glm::areCollinear(static_cast<Parent>(v1), static_cast<Parent>(v2), T(Math::Epsilon)); }
	static bool AreOrthogonal(const Vector3<T>& v1, const Vector3<T>& v2) { return glm::areOrthogonal(static_cast<Parent>(v1), static_cast<Parent>(v2), T(Math::Epsilon)); }
	static bool AreOrthonormal(const Vector3<T>& v1, const Vector3<T>& v2) { return glm::areOrthonormal(static_cast<Parent>(v1), static_cast<Parent>(v2), T(Math::Epsilon)); }
	
	// Min/Max
	Vector3<T>& Maximize(const Vector3<T>& v) { if (v.x > x) x = v.x; if (v.y > y) y = v.y; if (v.z > z) z = v.z; return *this; }
	static Vector3<T> Maximum(const Vector3<T>& v1, const Vector3<T>& v2) { return Vector3<T>(Math::Max(v1.x, v2.x), Math::Max(v1.y, v2.y), Math::Max(v1.z, v2.z)); }
	Vector3<T>& Minimize(const Vector3<T>& v) { if (v.x < x) x = v.x; if (v.y < y) y = v.y; if (v.z < z) z = v.z; return *this; }
	static Vector3<T> Minimum(const Vector3<T>& v1, const Vector3<T>& v2) { return Vector3<T>(Math::Min(v1.x, v2.x), Math::Min(v1.y, v2.y), Math::Min(v1.z, v2.z)); }

	// Lerp
	static Vector3<T> Lerp(const Vector3<T>& v1, const Vector3<T>& v2, T percent) { return Vector3(glm::lerp(static_cast<Parent>(v1), static_cast<Parent>(v2), percent)); }

	// Subsets
	constexpr Vector2<T> xy() const { return Vector2<T>(x, y); }
	constexpr Vector2<T> xz() const { return Vector2<T>(x, z); }
	constexpr Vector2<T> yz() const { return Vector2<T>(y, z); }

	// Meta
	bool Serialize(Serializer& serializer, const char* name);
	bool Edit(ObjectEditor& objectEditor, const char* name);
};

template <typename T> constexpr Vector3<T> operator+(T scalar, const Vector3<T>& v) { return Vector3(scalar + static_cast<Vector3<T>::Parent>(v)); }
template <typename T> constexpr Vector3<T> operator-(T scalar, const Vector3<T>& v) { return Vector3(scalar - static_cast<Vector3<T>::Parent>(v)); }
template <typename T> constexpr Vector3<T> operator*(T scalar, const Vector3<T>& v) { return Vector3(scalar * static_cast<Vector3<T>::Parent>(v)); }

template <typename T>
bool Vector3<T>::Serialize(Serializer& serializer, const char* name)
{
	if (serializer.BeginClass(name, TypeInfo<Vector3<T>>::GetName(), TypeInfo<Vector3<T>>::GetHash()))
	{
		bool ret = true;
		ret = GenericSerialization(serializer, "x", x) && ret;
		ret = GenericSerialization(serializer, "y", y) && ret;
		ret = GenericSerialization(serializer, "z", z) && ret;
		ret = serializer.EndClass() && ret;
		return ret;
	}
	else
	{
		return false;
	}
}

template <typename T>
bool Vector3<T>::Edit(ObjectEditor& objectEditor, const char* name)
{
#ifdef ENLIVE_ENABLE_IMGUI
	if (objectEditor.IsImGuiEditor())
	{
		if constexpr (Traits::IsFloatingPoint<T>::value)
		{
			float vector[3];
			vector[0] = static_cast<float>(x);
			vector[1] = static_cast<float>(y);
			vector[2] = static_cast<float>(z);
			if (ImGui::InputFloat3(name, vector))
			{
				Set(static_cast<T>(vector[0]), static_cast<T>(vector[1]), static_cast<T>(vector[2]));
				return true;
			}
			return false;
		}
		else
		{
			int vector[3];
			vector[0] = static_cast<int>(x);
			vector[1] = static_cast<int>(y);
			vector[2] = static_cast<int>(z);
			if (ImGui::InputInt3(name, vector))
			{
				// TODO : NumericLimits<T> Min
				// TODO : NumericLimits<T> Max
				Set(static_cast<T>(vector[0]), static_cast<T>(vector[1]), static_cast<T>(vector[2]));
				return true;
			}
			return false;
		}
	}
	else
#endif // ENLIVE_ENABLE_IMGUI
	{
		bool ret = false;
		if (objectEditor.BeginClass(name, TypeInfo<Vector3<T>>::GetName(), TypeInfo<Vector3<T>>::GetHash()))
		{
			ret = GenericEdit(objectEditor, "x", x) || ret;
			ret = GenericEdit(objectEditor, "y", y) || ret;
			ret = GenericEdit(objectEditor, "z", z) || ret;
			objectEditor.EndClass();
		}
		return ret;
	}
}

typedef Vector3<F32> Vector3f;
typedef Vector3<F64> Vector3d;
typedef Vector3<I32> Vector3i;
typedef Vector3<U32> Vector3u;

typedef Vector3f vec3; // GLSL-like

// These tests checks that the config for direction and handedness is correct
static_assert(ENLIVE_DEFAULT_FORWARD == -ENLIVE_DEFAULT_BACKWARD);
static_assert(ENLIVE_DEFAULT_UP == -ENLIVE_DEFAULT_DOWN);
static_assert(ENLIVE_DEFAULT_RIGHT == -ENLIVE_DEFAULT_LEFT);
static_assert((ENLIVE_DEFAULT_HANDEDNESS == en::Math::Handedness::Right && ENLIVE_DEFAULT_LEFT.Cross(ENLIVE_DEFAULT_UP) == ENLIVE_DEFAULT_FORWARD) || (ENLIVE_DEFAULT_HANDEDNESS == en::Math::Handedness::Left && ENLIVE_DEFAULT_RIGHT.Cross(ENLIVE_DEFAULT_UP) == ENLIVE_DEFAULT_FORWARD));
static_assert((ENLIVE_DEFAULT_HANDEDNESS == en::Math::Handedness::Right && ENLIVE_DEFAULT_UP.Cross(ENLIVE_DEFAULT_FORWARD) == ENLIVE_DEFAULT_LEFT) || (ENLIVE_DEFAULT_HANDEDNESS == en::Math::Handedness::Left && ENLIVE_DEFAULT_UP.Cross(ENLIVE_DEFAULT_FORWARD) == ENLIVE_DEFAULT_RIGHT));
static_assert((ENLIVE_DEFAULT_HANDEDNESS == en::Math::Handedness::Right && ENLIVE_DEFAULT_FORWARD.Cross(ENLIVE_DEFAULT_LEFT) == ENLIVE_DEFAULT_UP) || (ENLIVE_DEFAULT_HANDEDNESS == en::Math::Handedness::Left && ENLIVE_DEFAULT_FORWARD.Cross(ENLIVE_DEFAULT_RIGHT) == ENLIVE_DEFAULT_UP));

} // namespace en

ENLIVE_DEFINE_TYPE_INFO_TEMPLATE(en::Vector3, en::Type_CustomSerialization, en::Type_CustomEditor)