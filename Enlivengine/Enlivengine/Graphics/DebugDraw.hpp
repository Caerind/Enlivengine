#pragma once

#include <Enlivengine/System/PrimitiveTypes.hpp>
#include <Enlivengine/System/Config.hpp>

#ifdef ENLIVE_DEBUG

#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/CircleShape.hpp>
#include <SFML/Graphics/RectangleShape.hpp>

#include <Enlivengine/Graphics/Color.hpp>
#include <Enlivengine/Math/Rect.hpp>

namespace en
{

class DebugDraw
{
	public:
		DebugDraw() = delete;

		static void drawPoint(F32 x, F32 y, const Color& color = Color::Red, F32 r = 2.0f);
		static void drawPoint(const Vector2f& point, const Color& color = Color::Red, F32 r = 2.0f);
		static void drawRect(F32 x, F32 y, F32 w, F32 h, const Color& c1 = Color::Red, const Color& c2 = Color::Transparent);
		static void drawRect(const Rectf& rect, const Color& c1 = Color::Red, const Color& c2 = Color::Transparent);
		static void drawCircle(F32 x, F32 y, const Color& color = Color::Red, F32 r = 10.f); // TODO : Improve
		static void drawLine(F32 x1, F32 y1, F32 x2, F32 y2, const Color& color = Color::Red, F32 thickness = 2.0f); // TODO : Improve

		static U32 getCurrentRectangleCount();
		static U32 getCurrentCircleCount();
		static void render(sf::RenderTarget& target);

		static void reset();

		static void setMaxSize(U32 maxSize);
		static U32 getMaxSize();

		static void setVisible(bool visible);
		static bool isVisible();
		
	private:
		static bool mVisible;
		static U32 mMaxSize;
		static U32 mCurrentCircleIndex;
		static U32 mCurrentRectangleIndex;
		static std::vector<sf::CircleShape> mCircles;
		static std::vector<sf::RectangleShape> mRectangles;
};

} // namespace en

#endif // ENLIVE_DEBUG