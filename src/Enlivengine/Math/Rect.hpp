#pragma once

#include <Enlivengine/Math/Vector2.hpp>

namespace en 
{

template <typename T>
class Rect
{
public:
	constexpr Rect() : mMin(), mMax() {}
	constexpr Rect(const Vector2<T>& min, const Vector2<T>& max) : mMin(min), mMax(max) {}
	constexpr Rect(T minX, T minY, T sizeX, T sizeY) : mMin(minX, minY), mMax(minX + sizeX, minY + sizeY) {}

	constexpr const Vector2<T>& GetMin() const { return mMin; }
	constexpr void SetMin(const Vector2<T>& min) { mMin = min; }

	constexpr const Vector2<T>& GetMax() const { return mMax; }
	constexpr void SetMax(const Vector2<T>& max) { mMax = max; }

	constexpr T Left() const { return mMin.x; }
	constexpr T Top() const { return mMin.y; }
	constexpr T Right() const { return mMax.x; }
	constexpr T Bottom() const { return mMax.y; }
	constexpr T Width() const { return mMax.x - mMin.x; }
	constexpr T Height() const { return mMax.y - mMin.y; }

	constexpr Vector2<T> GetSize() const { return mMax - mMin; }
	constexpr void SetSize(const Vector2<T>& size) { mMax = mMin + size; }

	constexpr Vector2<T> GetCenter() const { return (mMin + mMax) * 0.5f; }
	constexpr void SetCenter(const Vector2<T>& center) { const Vector2<T> hs = GetHalfSize(); mMin = center - hs; mMax = center + hs; }

	constexpr Vector2<T> GetHalfSize() const { return (mMax - mMin) * 0.5f; }
	constexpr T GetArea() const { const Vector2f size = GetSize(); return size.x * size.y; }

	/*
	0---1
	|   |
	3---2
	*/
	constexpr Vector2<T> GetCorner(U32 index) const
	{
		switch (index)
		{
		case 0:
			return mMin;
		case 1:
			return Vector2<T>(mMax.x, mMin.y);
		case 2:
			return mMax;
		case 3:
			return Vector2<T>(mMin.x, mMax.y);
		default:
			break;
		}
		return Vector2<T>();
	}

	constexpr T GetSquaredDistance(const Vector2<T>& point) const
	{
		if (Contains(point))
		{
			return 0.0f;
		}
		else
		{
			Vector2<T> maxDist(0.0f);

			if (point.x < mMin.x)
				maxDist.x = mMin.x - point.x;
			else if (point.x > mMax.x)
				maxDist.x = point.x - mMax.x;

			if (point.y < mMin.y)
				maxDist.y = mMin.y - point.y;
			else if (point.y > mMax.y)
				maxDist.y = point.y - mMax.y;

			return maxDist.GetSquaredLength();
		}
	}
	inline T GetDistance(const Vector2<T>& point) const
	{
		return Math::FastSqrt(GetSquaredDistance(point));
	}

	constexpr bool Contains(T x, T y) const
	{
		return Math::InRange(x, mMin.x, mMax.x) && Math::InRange(y, mMin.y, mMax.y);
	}
	constexpr bool Contains(const Vector2<T>& point) const
	{
		return Math::InRange(point.x, mMin.x, mMax.x) && Math::InRange(point.y, mMin.y, mMax.y);
	}
	constexpr bool Contains(const Rect<T>& rect) const
	{
		return mMin.x <= rect.mMin.x
			&& mMin.y <= rect.mMin.y
			&& mMax.x >= rect.mMax.x
			&& mMax.y >= rect.mMax.y;
	}

	constexpr bool Intersects(const Rect<T>& rect, Rect<T>* intersection = nullptr) const
	{
		T left = Math::Max(mMin.x, rect.mMin.x);
		T right = Math::Min(mMax.x, rect.mMax.x);
		if (left >= right)
			return false;

		T top = Math::Max(mMin.y, rect.mMin.y);
		T bottom = Math::Min(mMax.y, rect.mMax.y);
		if (top >= bottom)
			return false;

		if (intersection != nullptr)
		{
			intersection->mMin.Set(left, top);
			intersection->mMax.Set(right, bottom);
		}

		return true;
	}

	constexpr bool operator==(const Rect<T>& other) const { return mMin == other.mMin && mMax == other.mMax; }
	constexpr bool operator!=(const Rect<T>& other) const { return !operator==(other); }

	bool Serialize(Serializer& serializer, const char* name);
	bool Edit(ObjectEditor& objectEditor, const char* name);

private:
	Vector2<T> mMin;
	Vector2<T> mMax;
};

template <typename T>
bool Rect<T>::Serialize(Serializer& serializer, const char* name)
{
	if (serializer.BeginClass(name, TypeInfo<Rect<T>>::GetName(), TypeInfo<Rect<T>>::GetHash()))
	{
		bool ret = true;
		ret = GenericSerialization(serializer, "min", mMin) && ret;
		ret = GenericSerialization(serializer, "max", mMax) && ret;
		ret = serializer.EndClass() && ret;
		return ret;
	}
	else
	{
		return false;
	}
}

template <typename T>
bool Rect<T>::Edit(ObjectEditor& objectEditor, const char* name)
{
	if (objectEditor.BeginClass(name, TypeInfo<Rect<T>>::GetName(), TypeInfo<Rect<T>>::GetHash()))
	{
		bool ret = false;
		ret = GenericEdit(objectEditor, "min", mMin) || ret;
		ret = GenericEdit(objectEditor, "max", mMax) || ret;
		objectEditor.EndClass();
		return ret;
	}
	else
	{
		return false;
	}
}

typedef Rect<F32> Rectf;
typedef Rect<I32> Recti;
typedef Rect<U32> Rectu;

} // namespace en

// We can't register template class for now, so I use custom serialization/editor instead
ENLIVE_DEFINE_TYPE_INFO_TEMPLATE(en::Rect, en::Type_CustomSerialization, en::Type_CustomEditor)