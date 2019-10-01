#include <Enlivengine/Graphics/DebugDraw.hpp>
#include <Enlivengine/Graphics/SFMLWrapper.hpp>

namespace en
{

/*

template<> DebugDraw* Singleton<DebugDraw>::mSingleton = nullptr;

DebugDraw::DebugDraw()
	: mVisible(true)
{
}

DebugDraw& DebugDraw::getSingleton()
{
	ASSERT(mSingleton != nullptr);
	return *mSingleton;
}

DebugDraw* DebugDraw::getSingletonPtr()
{
	return mSingleton;
}

bool DebugDraw::instanced()
{
	return mSingleton != nullptr;
}

void DebugDraw::clear()
{
	if (instanced())
	{
		U32 size = mSingleton->mDrawables.size();
		for (U32 i = 0; i < size; i++)
		{
			delete mSingleton->mDrawables[i];
		}
		mSingleton->mDrawables.clear();
	}
}

void DebugDraw::drawPoint(F32 x, F32 y, const Color& color, F32 r)
{
	if (instanced())
	{
		sf::CircleShape* point = new sf::CircleShape(r);
		point->setFillColor(toSF(color));
		point->setPosition(x - r * 0.5f, y - r * 0.5f);
		mSingleton->mDrawables.push_back(point);
	}
}

void DebugDraw::drawPoint(const oe::Vector2& point, const Color& color, F32 r)
{
	drawPoint(point.x, point.y, color, r);
}

void DebugDraw::drawRect(F32 x, F32 y, F32 w, F32 h, const Color& c1, const Color& c2)
{
	if (instanced())
	{
		sf::RectangleShape* rect = new sf::RectangleShape(sf::Vector2f(w, h));
		rect->setOutlineThickness(1.5f);
		rect->setOutlineColor(toSF(c1));
		rect->setFillColor(toSF(c2));
		rect->setPosition(x, y);
		mSingleton->mDrawables.push_back(rect);
	}
}

void DebugDraw::drawRect(const Rect& rect, const Color& c1, const Color& c2)
{
	drawRect(rect.x, rect.y, rect.w, rect.h, c1, c2);
}

void DebugDraw::drawCircle(F32 x, F32 y, const Color& color, F32 r)
{
	if (instanced())
	{
		sf::CircleShape* circle = new sf::CircleShape(r);
		circle->setFillColor(sf::Color::Transparent);
		circle->setOutlineColor(toSF(color));
		circle->setPosition(x - r * 0.5f, y - r * 0.5f);
		mSingleton->mDrawables.push_back(circle);
	}
}

void DebugDraw::drawLine(F32 x1, F32 y1, F32 x2, F32 y2, const Color& color, F32 thickness)
{
	if (instanced())
	{
		sf::RectangleShape* line = new sf::RectangleShape();
		line->setFillColor(sf::Color::Red);
		line->setPosition(x1, y1 - thickness * 0.5f);
		Vector2 v(x2 - x1, y2 - y1);
		line->setSize(sf::Vector2f(v.getLength(), thickness));
		line->setRotation(v.getPolarAngle());
		mSingleton->mDrawables.push_back(line);
	}
}

void DebugDraw::render(sf::RenderTarget& target)
{
	if (instanced())
	{
		U32 size = mSingleton->mDrawables.size();
		for (U32 i = 0; i < size; i++)
		{
			target.draw(*mSingleton->mDrawables[i]);
		}
	}
}

void DebugDraw::setVisible(bool visible)
{
	if (instanced())
	{
		getSingleton().mVisible = visible;
	}
}

bool DebugDraw::isVisible()
{
	if (instanced())
	{
		return getSingleton().mVisible;
	}
	return false;
}

*/

} // namespace en