#pragma once

#include <Enlivengine/Math/Math.hpp>

namespace en
{

template <typename T>
struct Vector2 : public glm::vec<2, T, glm::defaultp>
{
public:
	using Parent = glm::vec<2, T, glm::defaultp>;

	// Constructors
	constexpr Vector2() : Parent() {}
	constexpr Vector2(const Vector2<T>& v) : Parent(v.x, v.y) {}
	constexpr explicit Vector2(T scalar) : Parent(scalar) {}
	constexpr Vector2(T x, T y) : Parent(x, y) {}
	template <typename Other>
	constexpr Vector2(const Vector2<Other>& v) : Parent(v.x, v.y) {}
	template <typename OtherA, typename OtherB>
	constexpr Vector2(OtherA x, OtherB y) : Parent(x, y) {}
	constexpr Vector2(const Parent& parent) : Parent(parent) {}

	// Operators
	constexpr Vector2<T>& operator=(const Vector2<T>& v) { Parent::operator=(v); return *this; }
	constexpr Vector2<T> operator+() const { return Parent::operator+(); }
	constexpr Vector2<T> operator-() const { return Parent::operator-(); }
	constexpr Vector2<T>& operator+=(const Vector2<T>& v) { Parent::operator+=(v); return *this; }
	constexpr Vector2<T>& operator-=(const Vector2<T>& v) { Parent::operator-=(v); return *this; }
	constexpr Vector2<T>& operator*=(const Vector2<T>& v) { Parent::operator*=(v); return *this; }
	constexpr Vector2<T>& operator/=(const Vector2<T>& v) { Parent::operator/=(v); return *this; }
	constexpr Vector2<T>& operator+=(T scalar) { Parent::operator+=(scalar); return *this; }
	constexpr Vector2<T>& operator-=(T scalar) { Parent::operator-=(scalar); return *this; }
	constexpr Vector2<T>& operator*=(T scalar) { Parent::operator*=(scalar); return *this; }
	constexpr Vector2<T>& operator/=(T scalar) { Parent::operator/=(scalar); return *this; }
	constexpr Vector2<T> operator+(const Vector2<T>& v) const { return Vector2(); }
	constexpr Vector2<T> operator-(const Vector2<T>& v) const { return Vector2(static_cast<Parent>(*this) - static_cast<Parent>(v)); }
	constexpr Vector2<T> operator*(const Vector2<T>& v) const { return Vector2(static_cast<Parent>(*this) * static_cast<Parent>(v)); }
	constexpr Vector2<T> operator/(const Vector2<T>& v) const { return Vector2(static_cast<Parent>(*this) / static_cast<Parent>(v)); }
	constexpr Vector2<T> operator+(T scalar) const { return Vector2(static_cast<Parent>(*this) + scalar); }
	constexpr Vector2<T> operator-(T scalar) const { return Vector2(static_cast<Parent>(*this) - scalar); }
	constexpr Vector2<T> operator*(T scalar) const { return Vector2(static_cast<Parent>(*this) * scalar); }
	constexpr Vector2<T> operator/(T scalar) const { return Vector2(static_cast<Parent>(*this) / scalar); }
	constexpr bool operator==(const Vector2<T>& v) const { return static_cast<Parent>(*this) == static_cast<Parent>(v); }
	constexpr bool operator!=(const Vector2<T>& v) const { return static_cast<Parent>(*this) != static_cast<Parent>(v); }

	// Constants
	static constexpr Vector2<T> Unit() { return Vector2<T>(1, 1); }
	static constexpr Vector2<T> UnitX() { return Vector2<T>(1, 0); }
	static constexpr Vector2<T> UnitY() { return Vector2<T>(0, 1); }
	static constexpr Vector2<T> Zero() { return Vector2<T>(0, 0); }

	// Setters
	constexpr void Set(const Vector2<T>& v) { Parent::operator=(v); }
	template <typename U>
	constexpr void Set(const Vector2<U>& v) { Parent::operator=(v); }
	constexpr void Set(T scalar) { x = scalar; y = scalar; }
	constexpr void Set(T _x, T _y) { x = _x; y = _y; }
	template <typename OtherA, typename OtherB>
	constexpr void Set(OtherA _x, OtherB _y) { x = static_cast<T>(_x); y = static_cast<T>(_y); }

	// Meta
	bool Serialize(Serializer& serializer, const char* name);
	bool Edit(ObjectEditor& objectEditor, const char* name);
};

template <typename T> constexpr Vector2<T> operator+(T scalar, const Vector2<T>& v) { return Vector2(scalar + static_cast<Parent>(v)); }
template <typename T> constexpr Vector2<T> operator-(T scalar, const Vector2<T>& v) { return Vector2(scalar - static_cast<Parent>(v)); }
template <typename T> constexpr Vector2<T> operator*(T scalar, const Vector2<T>& v) { return Vector2(scalar * static_cast<Parent>(v)); }

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
				x = vector[0];
				y = vector[1];
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
				x = vector[0];
				y = vector[1];
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

typedef Vector2<F32> Vector2f;
typedef Vector2<F64> Vector2d;
typedef Vector2<I32> Vector2i;
typedef Vector2<U32> Vector2u;

typedef Vector2f vec2; // GLSL-like

} // namespace en

ENLIVE_DEFINE_TYPE_INFO_TEMPLATE(en::Vector2, en::Type_CustomSerialization, en::Type_CustomEditor)
