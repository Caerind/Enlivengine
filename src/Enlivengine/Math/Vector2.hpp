#pragma once

#include <Enlivengine/Math/Math.hpp>

#include <glm/gtx/vector_query.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include <glm/gtx/compatibility.hpp>
#include <glm/gtx/normalize_dot.hpp>

namespace en
{

template <typename T>
struct Vector2 : public glm::vec<2, T, glm::defaultp>
{
public:
	using Parent = glm::vec<2, T, glm::defaultp>;
	using ElementType = T;
	static constexpr glm::qualifier Precision = glm::defaultp;
	static constexpr U32 Dim = 2;

	// Constructors
	constexpr Vector2() : Parent() {}
	constexpr Vector2(const Vector2<T>& v) : Parent(static_cast<Parent>(v)) {}
	constexpr explicit Vector2(T scalar) : Parent(scalar) {}
	constexpr Vector2(T x, T y) : Parent(x, y) {}
	constexpr Vector2(const T* data) : Parent(data[0], data[1]) {}
	template <typename Other>
	constexpr Vector2(const Vector2<Other>& v) : Parent(static_cast<Vector2<Other>::Parent>(v)) {}
	template <typename OtherA, typename OtherB>
	constexpr Vector2(OtherA x, OtherB y) : Parent(x, y) {}
	constexpr Vector2(const Parent& parent) : Parent(parent) {}
	~Vector2() = default;

	// Operators
	constexpr Vector2<T>& operator=(const Vector2<T>& v) { Parent::operator=(v); return *this; }
	constexpr Vector2<T> operator+() const { return Vector2(+static_cast<Parent>(*this)); }
	constexpr Vector2<T> operator-() const { return Vector2(-static_cast<Parent>(*this)); }
	constexpr Vector2<T>& operator+=(const Vector2<T>& v) { Parent::operator+=(static_cast<Parent>(v)); return *this; }
	constexpr Vector2<T>& operator-=(const Vector2<T>& v) { Parent::operator-=(static_cast<Parent>(v)); return *this; }
	constexpr Vector2<T>& operator*=(const Vector2<T>& v) { Parent::operator*=(static_cast<Parent>(v)); return *this; }
	constexpr Vector2<T>& operator/=(const Vector2<T>& v) { Parent::operator/=(static_cast<Parent>(v)); return *this; }
	constexpr Vector2<T> operator+(const Vector2<T>& v) const { return Vector2(static_cast<Parent>(*this) + static_cast<Parent>(v)); }
	constexpr Vector2<T> operator-(const Vector2<T>& v) const { return Vector2(static_cast<Parent>(*this) - static_cast<Parent>(v)); }
	constexpr Vector2<T> operator*(const Vector2<T>& v) const { return Vector2(static_cast<Parent>(*this) * static_cast<Parent>(v)); }
	constexpr Vector2<T> operator/(const Vector2<T>& v) const { return Vector2(static_cast<Parent>(*this) / static_cast<Parent>(v)); }
	constexpr Vector2<T>& operator+=(T scalar) { Parent::operator+=(scalar); return *this; }
	constexpr Vector2<T>& operator-=(T scalar) { Parent::operator-=(scalar); return *this; }
	constexpr Vector2<T>& operator*=(T scalar) { Parent::operator*=(scalar); return *this; }
	constexpr Vector2<T>& operator/=(T scalar) { Parent::operator/=(scalar); return *this; }
	constexpr Vector2<T> operator+(T scalar) const { return Vector2(static_cast<Parent>(*this) + scalar); }
	constexpr Vector2<T> operator-(T scalar) const { return Vector2(static_cast<Parent>(*this) - scalar); }
	constexpr Vector2<T> operator*(T scalar) const { return Vector2(static_cast<Parent>(*this) * scalar); }
	constexpr Vector2<T> operator/(T scalar) const { return Vector2(static_cast<Parent>(*this) / scalar); }
	constexpr bool operator==(const Vector2<T>& v) const { return static_cast<Parent>(*this) == static_cast<Parent>(v); }
	constexpr bool operator!=(const Vector2<T>& v) const { return static_cast<Parent>(*this) != static_cast<Parent>(v); }
	constexpr bool operator<(const Vector2<T>& v) const { return x < v.x && y < v.y; }
	constexpr bool operator<=(const Vector2<T>& v) const { return x <= v.x && y <= v.y; }
	constexpr bool operator>(const Vector2<T>& v) const { return !operator<=(v); }
	constexpr bool operator>=(const Vector2<T>& v) const { return !operator<(v); }
	bool IsZero() const { return glm::isNull(static_cast<Parent>(*this), T(Math::Epsilon)); }
	static bool Equals(const Vector2<T>& v1, const Vector2<T>& v2, T epsilon = T(Math::Epsilon)) { return Math::Equals(v1.x, v2.x, epsilon) && Math::Equals(v1.y, v2.y, epsilon); }

	// Accessors
	T& operator()(U32 i) { if (i == 0) return x; return y; }
	const T& operator()(U32 i) const { if (i == 0) return x; return y; }
	T& operator[](U32 i) { if (i == 0) return x; return y; }
	const T& operator[](U32 i) const { if (i == 0) return x; return y; }
	T* GetData() { return const_cast<T*>(glm::value_ptr(static_cast<Parent>(*this))); }
	const T* GetData() const { return glm::value_ptr(static_cast<Parent>(*this)); }

	// Constants
	static constexpr Vector2<T> Unit() { return Vector2<T>(1, 1); }
	static constexpr Vector2<T> UnitX() { return Vector2<T>(1, 0); }
	static constexpr Vector2<T> UnitY() { return Vector2<T>(0, 1); }
	static constexpr Vector2<T> Zero() { return Vector2<T>(0, 0); }

	// Setters
	void Set(const Vector2<T>& v) { Parent::operator=(static_cast<Parent>(v)); }
	template <typename Other>
	void Set(const Vector2<Other>& v) { Parent::operator=(static_cast<Vector2<Other>::Parent>(v)); }
	void Set(T scalar) { x = scalar; y = scalar; }
	void Set(T _x, T _y) { x = _x; y = _y; }
	void Set(const T* data) { x = data[0]; y = data[1]; }
	template <typename OtherA, typename OtherB>
	void Set(OtherA _x, OtherB _y) { x = static_cast<T>(_x); y = static_cast<T>(_y); }

	// Norm
	T GetSquaredLength() const { return glm::length2(static_cast<Parent>(*this)); }
	T GetLength() const { return glm::length(static_cast<Parent>(*this)); }
	bool IsNormalized() const { return glm::isNormalized(static_cast<Parent>(*this), T(Math::Epsilon)); }
	Vector2<T>& FastNormalize() { *this = Vector2(glm::fastNormalize(static_cast<Parent>(*this))); return *this; }
	Vector2<T> FastNormalized() const { return Vector2(glm::fastNormalize(static_cast<Parent>(*this))); }
	Vector2<T>& Normalize() { *this = Vector2(glm::normalize(static_cast<Parent>(*this))); return *this; }
	Vector2<T> Normalized() const { return Vector2(glm::normalize(static_cast<Parent>(*this))); }

	// Operations
	constexpr T Dot(const Vector2<T>& v) const { return glm::dot(static_cast<Parent>(*this), static_cast<Parent>(v)); }
	static constexpr T Dot(const Vector2<T>& v1, const Vector2<T>& v2) { return glm::dot(static_cast<Parent>(v1), static_cast<Parent>(v2)); }
	T FastNormalizeDot(const Vector2<T>& v) const { return glm::fastNormalizeDot(static_cast<Parent>(*this), static_cast<Parent>(v)); }
	static T FastNormalizeDot(const Vector2<T>& v1, const Vector2<T>& v2) { return glm::fastNormalizeDot(static_cast<Parent>(v1), static_cast<Parent>(v2)); }
	T NormalizeDot(const Vector2<T>& v) const { return glm::normalizeDot(static_cast<Parent>(*this), static_cast<Parent>(v)); }
	static T NormalizeDot(const Vector2<T>& v1, const Vector2<T>& v2) { return glm::normalizeDot(static_cast<Parent>(v1), static_cast<Parent>(v2)); }
	static bool AreCollinear(const Vector2<T>& v1, const Vector2<T>& v2) { return glm::areCollinear(static_cast<Parent>(v1), static_cast<Parent>(v2), T(Math::Epsilon)); }
	static bool AreOrthogonal(const Vector2<T>& v1, const Vector2<T>& v2) { return glm::areOrthogonal(static_cast<Parent>(v1), static_cast<Parent>(v2), T(Math::Epsilon)); }
	static bool AreOrthonormal(const Vector2<T>& v1, const Vector2<T>& v2) { return glm::areOrthonormal(static_cast<Parent>(v1), static_cast<Parent>(v2), T(Math::Epsilon)); }

	// Min/Max
	Vector2<T>& Maximize(const Vector2<T>& v) { if (v.x > x) x = v.x; if (v.y > y) y = v.y; return *this; }
	static Vector2<T> Maximum(const Vector2<T>& v1, const Vector2<T>& v2) { return Vector2<T>(Math::Max(v1.x, v2.x), Math::Max(v1.y, v2.y)); }
	Vector2<T>& Minimize(const Vector2<T>& v) { if (v.x < x) x = v.x; if (v.y < y) y = v.y; return *this; }
	static Vector2<T> Minimum(const Vector2<T>& v1, const Vector2<T>& v2) { return Vector2<T>(Math::Min(v1.x, v2.x), Math::Min(v1.y, v2.y)); }

	// Lerp
	static Vector2<T> Lerp(const Vector2<T>& v1, const Vector2<T>& v2, T percent) { return Vector2(glm::lerp(static_cast<Parent>(v1), static_cast<Parent>(v2), percent)); }

	// PolarAngle
	T GetPolarAngle() const { return Math::Atan2(x, y); }
	Vector2<T>& SetPolarAngle(T angle) { const T length = GetLength(); x = Math::Cos(angle) * length; y = Math::Sin(angle) * length; return *this; }
	static Vector2<T> Polar(T angle, T length = T(1)) { return Vector2<T>(Math::Cos(angle) * length, Math::Sin(angle) * length); }
	Vector2<T>& Rotate(T angle) { *this = Vector2(glm::rotate(static_cast<Parent>(*this), angle * T(Math::kDegToRad))); return *this; }
	Vector2<T> Rotated(const T& angle) const { return Vector2(glm::rotate(static_cast<Parent>(*this), angle * T(Math::kDegToRad))); }

	// Meta
	bool Serialize(Serializer& serializer, const char* name);
	bool Edit(ObjectEditor& objectEditor, const char* name);
};

template <typename T> constexpr Vector2<T> operator+(T scalar, const Vector2<T>& v) { return Vector2(scalar + static_cast<Vector2<T>::Parent>(v)); }
template <typename T> constexpr Vector2<T> operator-(T scalar, const Vector2<T>& v) { return Vector2(scalar - static_cast<Vector2<T>::Parent>(v)); }
template <typename T> constexpr Vector2<T> operator*(T scalar, const Vector2<T>& v) { return Vector2(scalar * static_cast<Vector2<T>::Parent>(v)); }

template <typename T>
bool Vector2<T>::Serialize(Serializer& serializer, const char* name)
{
	if (serializer.BeginClass(name, TypeInfo<Vector2<T>>::GetName(), TypeInfo<Vector2<T>>::GetHash()))
	{
		bool ret = true;
		ret = GenericSerialization(serializer, "x", x) && ret;
		ret = GenericSerialization(serializer, "y", y) && ret;
		ret = serializer.EndClass() && ret;
		return ret;
	}
	else
	{
		return false;
	}
}

template <typename T>
bool Vector2<T>::Edit(ObjectEditor& objectEditor, const char* name)
{
#ifdef ENLIVE_ENABLE_IMGUI
	if (objectEditor.IsImGuiEditor())
	{
		if constexpr (Traits::IsFloatingPoint<T>::value)
		{
			float vector[2];
			vector[0] = static_cast<float>(x);
			vector[1] = static_cast<float>(y);
			if (ImGui::InputFloat2(name, vector))
			{
				Set(static_cast<T>(vector[0]), static_cast<T>(vector[1]));
				return true;
			}
			return false;
		}
		else
		{
			int vector[2];
			vector[0] = static_cast<int>(x);
			vector[1] = static_cast<int>(y);
			if (ImGui::InputInt2(name, vector))
			{
				// TODO : NumericLimits<T> Min
				// TODO : NumericLimits<T> Max
				Set(static_cast<T>(vector[0]), static_cast<T>(vector[1]));
				return true;
			}
			return false;
		}
	}
	else
#endif // ENLIVE_ENABLE_IMGUI
	{
		bool ret = false;
		if (objectEditor.BeginClass(name, TypeInfo<Vector2<T>>::GetName(), TypeInfo<Vector2<T>>::GetHash()))
		{
			ret = GenericEdit(objectEditor, "x", x) || ret;
			ret = GenericEdit(objectEditor, "y", y) || ret;
			objectEditor.EndClass();
		}
		return ret;
	}
}

/*
template <typename T>
std::string ToString<Vector2<T>>(const Vector2<T>& v)
{
	return glm::to_string(static_cast<Vector2<T>::Parent>(v));
}
*/

typedef Vector2<F32> Vector2f;
typedef Vector2<F64> Vector2d;
typedef Vector2<I32> Vector2i;
typedef Vector2<U32> Vector2u;

typedef Vector2f vec2; // GLSL-like

} // namespace en

ENLIVE_DEFINE_TYPE_INFO_TEMPLATE(en::Vector2, en::Type_CustomSerialization, en::Type_CustomEditor)
