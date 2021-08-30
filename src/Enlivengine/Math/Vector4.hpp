#pragma once

#include <Enlivengine/Math/Vector3.hpp>

namespace en
{

template <typename T>
struct Vector4 : public glm::vec<4, T, glm::defaultp>
{
public:
	using Parent = glm::vec<4, T, glm::defaultp>;
	using ElementType = T;
	static constexpr glm::qualifier Precision = glm::defaultp;
	static constexpr U32 Dim = 4;

	// Constructors
	constexpr Vector4() : Parent() {}
	constexpr Vector4(const Vector4<T>& v) : Parent(static_cast<Parent>(v)) {}
	constexpr explicit Vector4(T scalar) : Parent(scalar) {}
	constexpr Vector4(T x, T y, T z, T w) : Parent(x, y, z, w) {}
	constexpr Vector4(const T* data) : Parent(data[0], data[1], data[2], data[3]) {}
	template <typename Other>
	constexpr Vector4(const Vector4<Other>& v) : Parent(static_cast<Vector4<Other>::Parent>(v)) {}
	template <typename OtherA, typename OtherB, typename OtherC, typename OtherD>
	constexpr Vector4(OtherA x, OtherB y, OtherC z, OtherD w) : Parent(x, y, z, w) {}
	constexpr Vector4(const Parent& parent) : Parent(parent) {}
	constexpr Vector4(const Vector3<T>& v, T w) : Parent(static_cast<Vector3<T>::Parent>(v), w) {}
	~Vector4() = default;

	// Operators
	constexpr Vector4<T>& operator=(const Vector4<T>& v) { Parent::operator=(static_cast<Parent>(v)); return *this; }
	constexpr Vector4<T> operator+() const { return Vector4(+static_cast<Parent>(*this)); }
	constexpr Vector4<T> operator-() const { return Vector4(-static_cast<Parent>(*this)); }
	constexpr Vector4<T>& operator+=(const Vector4<T>& v) { Parent::operator+=(static_cast<Parent>(v)); return *this; }
	constexpr Vector4<T>& operator-=(const Vector4<T>& v) { Parent::operator-=(static_cast<Parent>(v)); return *this; }
	constexpr Vector4<T>& operator*=(const Vector4<T>& v) { Parent::operator*=(static_cast<Parent>(v)); return *this; }
	constexpr Vector4<T>& operator/=(const Vector4<T>& v) { Parent::operator/=(static_cast<Parent>(v)); return *this; }
	constexpr Vector4<T> operator+(const Vector4<T>& v) const { return Vector4(static_cast<Parent>(*this) + static_cast<Parent>(v)); }
	constexpr Vector4<T> operator-(const Vector4<T>& v) const { return Vector4(static_cast<Parent>(*this) - static_cast<Parent>(v)); }
	constexpr Vector4<T> operator*(const Vector4<T>& v) const { return Vector4(static_cast<Parent>(*this) * static_cast<Parent>(v)); }
	constexpr Vector4<T> operator/(const Vector4<T>& v) const { return Vector4(static_cast<Parent>(*this) / static_cast<Parent>(v)); }
	constexpr Vector4<T>& operator+=(T scalar) { Parent::operator+=(scalar); return *this; }
	constexpr Vector4<T>& operator-=(T scalar) { Parent::operator-=(scalar); return *this; }
	constexpr Vector4<T>& operator*=(T scalar) { Parent::operator*=(scalar); return *this; }
	constexpr Vector4<T>& operator/=(T scalar) { Parent::operator/=(scalar); return *this; }
	constexpr Vector4<T> operator+(T scalar) const { return Vector4(static_cast<Parent>(*this) + scalar); }
	constexpr Vector4<T> operator-(T scalar) const { return Vector4(static_cast<Parent>(*this) - scalar); }
	constexpr Vector4<T> operator*(T scalar) const { return Vector4(static_cast<Parent>(*this) * scalar); }
	constexpr Vector4<T> operator/(T scalar) const { return Vector4(static_cast<Parent>(*this) / scalar); }
	constexpr bool operator==(const Vector4<T>& v) const { return static_cast<Parent>(*this) == static_cast<Parent>(v); }
	constexpr bool operator!=(const Vector4<T>& v) const { return static_cast<Parent>(*this) != static_cast<Parent>(v); }
	constexpr bool operator<(const Vector4<T>& v) const { return x < v.x && y < v.y&& z < v.z; }
	constexpr bool operator<=(const Vector4<T>& v) const { return x <= v.x && y <= v.y && z <= v.z; }
	constexpr bool operator>(const Vector4<T>& v) const { return !operator<=(v); }
	constexpr bool operator>=(const Vector4<T>& v) const { return !operator<(v); }
	bool IsZero() const { return glm::isNull(static_cast<Parent>(*this), T(Math::Epsilon)); }
	static bool Equals(const Vector4<T>& v1, const Vector4<T>& v2, T epsilon = T(Math::Epsilon)) { return Math::Equals(v1.x, v2.x, epsilon) && Math::Equals(v1.y, v2.y, epsilon) && Math::Equals(v1.z, v2.z, epsilon) && Math::Equals(v1.w, v2.w, epsilon); }

	// Accessors
	T& operator()(U32 i) { if (i == 0) return x; else if (i == 1) return y; else if (i == 2) return z; return w; }
	const T& operator()(U32 i) const { if (i == 0) return x; else if (i == 1) return y; else if (i == 2) return z; return w; }
	T& operator[](U32 i) { if (i == 0) return x; else if (i == 1) return y; else if (i == 2) return z; return w; }
	const T& operator[](U32 i) const { if (i == 0) return x; else if (i == 1) return y; else if (i == 2) return z; return w; }
	T* GetData() { return const_cast<T*>(glm::value_ptr(static_cast<Parent>(*this))); }
	const T* GetData() const { return glm::value_ptr(static_cast<Parent>(*this)); }

	// Constants
	static constexpr Vector4<T> Unit() { return Vector4<T>(1, 1, 1, 1); }
	static constexpr Vector4<T> UnitX() { return Vector4<T>(1, 0, 0, 0); }
	static constexpr Vector4<T> UnitY() { return Vector4<T>(0, 1, 0, 0); }
	static constexpr Vector4<T> UnitZ() { return Vector4<T>(0, 0, 1, 0); }
	static constexpr Vector4<T> UnitW() { return Vector4<T>(0, 0, 0, 1); }
	static constexpr Vector4<T> Zero() { return Vector4<T>(0, 0, 0, 0); }

	// Setters
	void Set(const Vector4<T>& v) { Parent::operator=(static_cast<Parent>(v)); }
	template <typename Other>
	void Set(const Vector4<Other>& v) { Parent::operator=(static_cast<Vector4<Other>::Parent>(v)); }
	void Set(T scalar) { x = scalar; y = scalar; z = scalar; w = scalar; }
	void Set(T _x, T _y, T _z, T _w) { x = _x; y = _y; z = _z; w = _w; }
	void Set(const T* data) { x = data[0]; y = data[1]; z = data[2]; w = data[3]; }
	template <typename OtherA, typename OtherB, typename OtherC, typename OtherD>
	void Set(OtherA _x, OtherB _y, OtherC _z, OtherD _w) { x = static_cast<T>(_x); y = static_cast<T>(_y); z = static_cast<T>(_z); w = static_cast<T>(_w); }
	void Set(const Vector3<T>& v, T _w) { x = v.x; y = v.y; z = v.z; w = _w; }

	// Norm
	T GetSquaredLength() const { return glm::length2(static_cast<Parent>(*this)); }
	T GetLength() const { return glm::length(static_cast<Parent>(*this)); }
	bool IsNormalized() const { return glm::isNormalized(static_cast<Parent>(*this), T(Math::Epsilon)); }
	Vector4<T>& FastNormalize() { *this = Vector4(glm::fastNormalize(static_cast<Parent>(*this))); return *this; }
	Vector4<T> FastNormalized() const { return Vector4(glm::fastNormalize(static_cast<Parent>(*this))); }
	Vector4<T>& Normalize() { *this = Vector4(glm::normalize(static_cast<Parent>(*this))); return *this; }
	Vector4<T> Normalized() const { return Vector4(glm::normalize(static_cast<Parent>(*this))); }

	// Operations
	constexpr T Dot(const Vector4<T>& v) const { return glm::dot(static_cast<Parent>(*this), static_cast<Parent>(v)); }
	static constexpr T Dot(const Vector4<T>& v1, const Vector4<T>& v2) { return glm::dot(static_cast<Parent>(v1), static_cast<Parent>(v2)); }
	T FastNormalizeDot(const Vector4<T>& v) const { return glm::fastNormalizeDot(static_cast<Parent>(*this), static_cast<Parent>(v)); }
	static T FastNormalizeDot(const Vector4<T>& v1, const Vector4<T>& v2) { return glm::fastNormalizeDot(static_cast<Parent>(v1), static_cast<Parent>(v2)); }
	T NormalizeDot(const Vector4<T>& v) const { return glm::normalizeDot(static_cast<Parent>(*this), static_cast<Parent>(v)); }
	static T NormalizeDot(const Vector4<T>& v1, const Vector4<T>& v2) { return glm::normalizeDot(static_cast<Parent>(v1), static_cast<Parent>(v2)); }
	static bool AreCollinear(const Vector4<T>& v1, const Vector4<T>& v2) { return glm::areCollinear(static_cast<Parent>(v1), static_cast<Parent>(v2), T(Math::Epsilon)); }
	static bool AreOrthogonal(const Vector4<T>& v1, const Vector4<T>& v2) { return glm::areOrthogonal(static_cast<Parent>(v1), static_cast<Parent>(v2), T(Math::Epsilon)); }
	static bool AreOrthonormal(const Vector4<T>& v1, const Vector4<T>& v2) { return glm::areOrthonormal(static_cast<Parent>(v1), static_cast<Parent>(v2), T(Math::Epsilon)); }

	// Min/Max
	constexpr Vector4<T>& Maximize(const Vector4<T>& v) { if (v.x > x) x = v.x; if (v.y > y) y = v.y; if (v.z > z) z = v.z; if (v.w > w) w = v.w; return *this; }
	static constexpr Vector4<T> Maximum(const Vector4<T>& v1, const Vector4<T>& v2) { return Vector3<T>(Math::Max(v1.x, v2.x), Math::Max(v1.y, v2.y), Math::Max(v1.z, v2.z), Math::Max(v1.w, v2.w)); }
	constexpr Vector4<T>& Minimize(const Vector4<T>& v) { if (v.x < x) x = v.x; if (v.y < y) y = v.y; if (v.z < z) z = v.z; if (v.w < w) w = v.w; return *this; }
	static constexpr Vector4<T> Minimum(const Vector4<T>& v1, const Vector4<T>& v2) { return Vector3<T>(Math::Min(v1.x, v2.x), Math::Min(v1.y, v2.y), Math::Min(v1.z, v2.z), Math::Min(v1.w, v2.w)); }

	// Lerp
	static constexpr Vector4<T> Lerp(const Vector4<T>& v1, const Vector4<T>& v2, T percent) { return Vector4(glm::lerp(static_cast<Parent>(v1), static_cast<Parent>(v2), percent)); }

	// Subsets
	constexpr Vector3<T> xyz() const { return Vector3<T>(x, y, z); }
	constexpr Vector2<T> xy() const { return Vector2<T>(x, y); }
	constexpr Vector2<T> xz() const { return Vector2<T>(x, z); }
	constexpr Vector2<T> yz() const { return Vector2<T>(y, z); }

	// Meta
	bool Serialize(Serializer& serializer, const char* name);
	bool Edit(ObjectEditor& objectEditor, const char* name);
};

template <typename T> constexpr Vector4<T> operator+(T scalar, const Vector4<T>& v) { return Vector4(scalar + static_cast<Vector4<T>::Parent>(v)); }
template <typename T> constexpr Vector4<T> operator-(T scalar, const Vector4<T>& v) { return Vector4(scalar - static_cast<Vector4<T>::Parent>(v)); }
template <typename T> constexpr Vector4<T> operator*(T scalar, const Vector4<T>& v) { return Vector4(scalar * static_cast<Vector4<T>::Parent>(v)); }

template <typename T>
bool Vector4<T>::Serialize(Serializer& serializer, const char* name)
{
	if (serializer.BeginClass(name, TypeInfo<Vector4<T>>::GetName(), TypeInfo<Vector4<T>>::GetHash()))
	{
		bool ret = true;
		ret = GenericSerialization(serializer, "x", x) && ret;
		ret = GenericSerialization(serializer, "y", y) && ret;
		ret = GenericSerialization(serializer, "z", z) && ret;
		ret = GenericSerialization(serializer, "w", w) && ret;
		ret = serializer.EndClass() && ret;
		return ret;
	}
	else
	{
		return false;
	}
}

template <typename T>
bool Vector4<T>::Edit(ObjectEditor& objectEditor, const char* name)
{
#ifdef ENLIVE_ENABLE_IMGUI
	if (objectEditor.IsImGuiEditor())
	{
		if constexpr (Traits::IsFloatingPoint<T>::value)
		{
			float vector[4];
			vector[0] = static_cast<float>(x);
			vector[1] = static_cast<float>(y);
			vector[2] = static_cast<float>(z);
			vector[3] = static_cast<float>(w);
			if (ImGui::InputFloat4(name, vector))
			{
				Set(static_cast<T>(vector[0]), static_cast<T>(vector[1]), static_cast<T>(vector[2]), static_cast<T>(vector[3]));
				return true;
			}
			return false;
		}
		else
		{
			int vector[4];
			vector[0] = static_cast<int>(x);
			vector[1] = static_cast<int>(y);
			vector[2] = static_cast<int>(z);
			vector[3] = static_cast<int>(w);
			if (ImGui::InputInt4(name, vector))
			{
				// TODO : NumericLimits<T> Min
				// TODO : NumericLimits<T> Max
				Set(static_cast<T>(vector[0]), static_cast<T>(vector[1]), static_cast<T>(vector[2]), static_cast<T>(vector[3]));
				return true;
			}
			return false;
		}
	}
	else
#endif // ENLIVE_ENABLE_IMGUI
	{
		bool ret = false;
		if (objectEditor.BeginClass(name, TypeInfo<Vector4<T>>::GetName(), TypeInfo<Vector4<T>>::GetHash()))
		{
			ret = GenericEdit(objectEditor, "x", x) || ret;
			ret = GenericEdit(objectEditor, "y", y) || ret;
			ret = GenericEdit(objectEditor, "z", z) || ret;
			ret = GenericEdit(objectEditor, "w", w) || ret;
			objectEditor.EndClass();
		}
		return ret;
	}
}

/*
template <typename T>
std::string ToString<Vector4<T>>(const Vector4<T>& v)
{
	return glm::to_string(static_cast<Vector4<T>::Parent>(v));
}
*/

typedef Vector4<F32> Vector4f;
typedef Vector4<F64> Vector4d;
typedef Vector4<I32> Vector4i;
typedef Vector4<U32> Vector4u;

typedef Vector4f vec4; // GLSL-like

} // namespace en

ENLIVE_DEFINE_TYPE_INFO_TEMPLATE(en::Vector4, en::Type_CustomSerialization, en::Type_CustomEditor)