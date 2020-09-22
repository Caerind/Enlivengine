#pragma once

#include <Enlivengine/Math/Vector2.hpp>

// TODO : constexpr

namespace en 
{

template <typename T>
class Rect
{
	public:
		Rect();
		Rect(const Vector2<T>& min, const Vector2<T>& size);
		Rect(T minX, T minY, T sizeX, T sizeY);

		const Vector2<T>& getMinimum() const;
		void setMinimum(const Vector2<T>& minimum);
		void setMinimum(T x, T y);

		const Vector2<T>& getSize() const;
		void setSize(const Vector2<T>& size);
		void setSize(T sizeX, T sizeY);

		Vector2<T> getMaximum() const;
		void setMaximum(const Vector2<T>& maximum);
		void setMaximum(T x, T y);

		T left() const;
		T top() const;
		T right() const;
		T bottom() const;
		T width() const;
		T height() const;

		void merge(T x, T y);
		void merge(const Vector2<T>& point);
		void merge(const Rect<T>& rect);

		void translate(const Vector2<T>& t);

		Vector2<T> getCenter() const;
		Vector2<T> getHalfSize() const;
		T getArea() const;

		/*
		0---1
		|   |
		3---2
		*/
		Vector2<T> getCorner(U8 index) const;

		T getDistanceSquared(const Vector2<T>& point) const;
		T getDistance(const Vector2<T>& point) const;

		bool contains(T x, T y) const;
		bool contains(const Vector2<T>& point) const;
		bool contains(const Rect<T>& rect) const;

		bool intersects(const Rect<T>& rect, Rect<T>* intersection = nullptr) const;

		bool operator==(const Rect<T>& rect) const;
		bool operator!=(const Rect<T>& rect) const;

	private:
		Vector2<T> mMin;
		Vector2<T> mSize;
};

typedef Rect<F32> Rectf;
typedef Rect<I32> Recti;
typedef Rect<U32> Rectu;

template <typename T>
Rect<T>::Rect()
	: mMin(0, 0)
	, mSize(0, 0)
{
}

template <typename T>
Rect<T>::Rect(const Vector2<T>& min, const Vector2<T>& size)
	: mMin(min)
	, mSize(size)
{
}

template <typename T>
Rect<T>::Rect(T minX, T minY, T sizeX, T sizeY)
	: mMin(minX, minY)
	, mSize(sizeX, sizeY)
{
}

template <typename T>
const Vector2<T>& Rect<T>::getMinimum() const
{
	return mMin;
}

template <typename T>
void Rect<T>::setMinimum(const Vector2<T>& minimum)
{
	mMin.Set(minimum);
}

template <typename T>
void Rect<T>::setMinimum(T x, T y)
{
	mMin.Set(x, y);
}

template <typename T>
const Vector2<T>& Rect<T>::getSize() const
{
	return mSize;
}

template <typename T>
void Rect<T>::setSize(const Vector2<T>& size)
{
	mSize.Set(size);
}

template <typename T>
void Rect<T>::setSize(T x, T y)
{
	mSize.Set(x, y);
}

template <typename T>
Vector2<T> Rect<T>::getMaximum() const
{
	return mMin + mSize;
}

template <typename T>
void Rect<T>::setMaximum(const Vector2<T>& maximum)
{
	mSize = maximum - mMin;
}

template <typename T>
void Rect<T>::setMaximum(T x, T y)
{
	setMaximum(Vector2<T>(x, y));
}

template<typename T>
inline T Rect<T>::left() const
{
	return mMin.x;
}

template<typename T>
inline T Rect<T>::top() const
{
	return mMin.y;
}

template<typename T>
inline T Rect<T>::right() const
{
	return mMin.x + mSize.x;
}

template<typename T>
inline T Rect<T>::bottom() const
{
	return mMin.y + mSize.y;
}

template<typename T>
inline T Rect<T>::width() const
{
	return mSize.x;
}

template<typename T>
inline T Rect<T>::height() const
{
	return mSize.y;
}

template <typename T>
void Rect<T>::merge(T x, T y)
{
	merge(Vector2<T>(x, y));
}

template <typename T>
void Rect<T>::merge(const Vector2<T>& point)
{
	mMin.Minimize(point);
	setMaximum(getMaximum().Maximize(point));
}

template <typename T>
void Rect<T>::merge(const Rect<T>& rect)
{
	mMin.Minimize(rect.mMin);
	setMaximum(getMaximum().Maximize(rect.getMaximum()));
}

template <typename T>
void Rect<T>::translate(const Vector2<T>& t)
{
	mMin += t;
}

template <typename T>
Vector2<T> Rect<T>::getCenter() const
{
	return mMin + getHalfSize();
}

template <typename T>
Vector2<T> Rect<T>::getHalfSize() const
{
	return mSize / 2;
}

template <typename T>
T Rect<T>::getArea() const
{
	return mSize.x * mSize.y;
}

template <typename T>
Vector2<T> Rect<T>::getCorner(U8 index) const
{
	/*
	0---1
	|   |
	3---2
	*/
	switch (index)
	{
	case 0:
		return mMin;
	case 1:
		return Vector2<T>(mMin.x + mSize.x, mMin.y);
	case 2:
		return mMin + mSize;
	case 3:
		return Vector2<T>(mMin.x, mMin.y + mSize.y);
	default:
		return Vector2<T>();
	}
}

template <typename T>
T Rect<T>::getDistanceSquared(const Vector2<T>& point) const
{
	if (contains(point))
	{
		return 0.0f;
	}
	else
	{
		Vector2<T> maxDist(0);
		T x = point.x;
		T y = point.y;

		Vector2<T> max = getMaximum();

		if (x < mMin.x)
			maxDist.x = mMin.x - x;
		else if (x > max.x)
			maxDist.x = x - max.x;

		if (y < mMin.y)
			maxDist.y = mMin.y - y;
		else if (y > max.y)
			maxDist.y = y - max.y;

		return maxDist.GetSquaredLength();
	}
}

template <typename T>
T Rect<T>::getDistance(const Vector2<T>& point) const
{
	return Math::Sqrt(getDistanceSquared(point));
}

template<typename T>
inline bool Rect<T>::contains(T x, T y) const
{
	Vector2<T> max = getMaximum();
	return Math::InRange(x, mMin.x, max.x) && Math::InRange(y, mMin.y, max.y);
}

template <typename T>
bool Rect<T>::contains(const Vector2<T>& point) const
{
	Vector2<T> max = getMaximum();
	return Math::InRange(point.x, mMin.x, max.x) && Math::InRange(point.y, mMin.y, max.y);
}

template<typename T>
inline bool Rect<T>::contains(const Rect<T>& rect) const
{
	Vector2<T> max = getMaximum();
	return mMin < rect.mMin && max > rect.getMaximum();
}

template<typename T>
inline bool Rect<T>::intersects(const Rect<T>& rect, Rect<T>* intersection) const
{
	Vector2<T> max = getMaximum();
	Vector2<T> max2 = rect.getMaximum();

	T left = Math::Max(mMin.x, rect.mMin.x);
	T right = Math::Min(max.x, max2.x);
	if (left >= right)
		return false;

	T top = Math::Max(mMin.y, rect.mMin.y);
	T bottom = Math::Min(max.y, max2.y);
	if (top >= bottom)
		return false;

	if (intersection != nullptr)
	{
		intersection->mMin.Set(left, top);
		intersection->mSize.Set(right - left, bottom - top);
	}

	return true;
}

template <typename T>
bool Rect<T>::operator==(const Rect<T>& rect) const
{
	return mMin == rect.mMin && mSize == rect.mSize;
}

template <typename T>
bool Rect<T>::operator!=(const Rect<T>& rect) const
{
	return !operator==(rect);
}

} // namespace en

ENLIVE_DEFINE_TYPE_INFO_TEMPLATE(en::Rect)
