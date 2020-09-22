#include <Enlivengine/Graphics/DebugDraw.hpp>

#ifdef ENLIVE_DEBUG

#include <Enlivengine/System/Profiler.hpp>
#include <Enlivengine/Graphics/SFMLWrapper.hpp>

namespace en
{

DebugDraw::DebugDraw()
	: mVisible(true)
	, mMaxSize(32)
	, mCurrentCircleIndex(0)
	, mCurrentRectangleIndex(0)
	, mCircles()
	, mRectangles()
{
	mCircles.resize(mMaxSize);
	mRectangles.resize(mMaxSize);
}

void DebugDraw::drawPoint(F32 x, F32 y, const Color& color, F32 r)
{
	if (mVisible && mCurrentCircleIndex < mMaxSize)
	{
		mCurrentCircleIndex++;
		sf::CircleShape& circle = mCircles.back();
		circle.setFillColor(toSF(color));
		circle.setPosition(x - r * 0.5f, y - r * 0.5f);
	}
}

void DebugDraw::drawPoint(const Vector2f& point, const Color& color, F32 r)
{
	drawPoint(point.x, point.y, color, r);
}

void DebugDraw::drawRect(F32 x, F32 y, F32 w, F32 h, const Color& c1, const Color& c2)
{
	if (mVisible && mCurrentRectangleIndex < mMaxSize)
	{
		mCurrentRectangleIndex++;
		sf::RectangleShape& rectangle = mRectangles.back();
		rectangle.setOutlineThickness(1.5f);
		rectangle.setOutlineColor(toSF(c1));
		rectangle.setFillColor(toSF(c2));
		rectangle.setPosition(x, y);
		rectangle.setSize({w, h});
	}
}

void DebugDraw::drawRect(const Rectf& rect, const Color& c1, const Color& c2)
{
	drawRect(rect.left(), rect.top(), rect.width(), rect.height(), c1, c2);
}

void DebugDraw::drawCircle(F32 x, F32 y, const Color& color, F32 r)
{
	if (mVisible && mCurrentCircleIndex < mMaxSize)
	{
		mCurrentCircleIndex++;
		sf::CircleShape& circle = mCircles.back();
		circle.setFillColor(sf::Color::Transparent);
		circle.setOutlineColor(toSF(color));
		circle.setPosition(x - r * 0.5f, y - r * 0.5f);
	}
}

void DebugDraw::drawLine(F32 x1, F32 y1, F32 x2, F32 y2, const Color& color, F32 thickness)
{
	if (mVisible && mCurrentRectangleIndex < mMaxSize)
	{
		mCurrentRectangleIndex++;
		sf::RectangleShape& rectangle = mRectangles.back();
		rectangle.setFillColor(toSF(color));
		rectangle.setPosition(x1, y1 - thickness * 0.5f);
		const Vector2f v(x2 - x1, y2 - y1);
		rectangle.setSize(sf::Vector2f(v.GetLength(), thickness));
		rectangle.setRotation(v.GetPolarAngle());
	}
}

U32 DebugDraw::getCurrentRectangleCount()
{
	return mCurrentRectangleIndex;
}

U32 DebugDraw::getCurrentCircleCount()
{
	return mCurrentCircleIndex;
}

void DebugDraw::render(sf::RenderTarget& target)
{
	ENLIVE_PROFILE_FUNCTION();

	if (mVisible)
	{
		for (U32 i = 0; i < mCurrentCircleIndex; ++i)
		{
			target.draw(mCircles[i]);
		}

		for (U32 i = 0; i < mCurrentRectangleIndex; ++i)
		{
			target.draw(mRectangles[i]);
		}
	}
}

void DebugDraw::reset()
{
	mCircles.clear();
	mRectangles.clear();
	mCurrentCircleIndex = 0;
	mCurrentRectangleIndex = 0;
}

void DebugDraw::setMaxSize(U32 maxSize)
{
	mMaxSize = maxSize;
}

U32 DebugDraw::getMaxSize()
{
	return mMaxSize;
}

void DebugDraw::setVisible(bool visible)
{
	mVisible = visible;
}

bool DebugDraw::isVisible()
{
	return mVisible;
}

} // namespace en

#endif // ENLIVE_DEBUG