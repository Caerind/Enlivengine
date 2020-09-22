#pragma once

#include <Enlivengine/System/PrimitiveTypes.hpp>
#include <Enlivengine/Config.hpp>

#ifdef ENLIVE_DEBUG

#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/CircleShape.hpp>
#include <SFML/Graphics/RectangleShape.hpp>

#include <Enlivengine/Math/Color.hpp>
#include <Enlivengine/Math/Rect.hpp>

namespace en
{

class DebugDraw
{
	private:
		DebugDraw();

	public:
		static DebugDraw& GetInstance() { static DebugDraw instance; return instance; }

		void drawPoint(F32 x, F32 y, const Color& color = Color::Red, F32 r = 2.0f);
		void drawPoint(const Vector2f& point, const Color& color = Color::Red, F32 r = 2.0f);
		void drawRect(F32 x, F32 y, F32 w, F32 h, const Color& c1 = Color::Red, const Color& c2 = Color::Transparent);
		void drawRect(const Rectf& rect, const Color& c1 = Color::Red, const Color& c2 = Color::Transparent);
		void drawCircle(F32 x, F32 y, const Color& color = Color::Red, F32 r = 10.f); // TODO : Improve
		void drawLine(F32 x1, F32 y1, F32 x2, F32 y2, const Color& color = Color::Red, F32 thickness = 2.0f); // TODO : Improve

		U32 getCurrentRectangleCount();
		U32 getCurrentCircleCount();
		void render(sf::RenderTarget& target);

		void reset();

		void setMaxSize(U32 maxSize);
		U32 getMaxSize();

		void setVisible(bool visible);
		bool isVisible();
		
	private:
		bool mVisible;
		U32 mMaxSize;
		U32 mCurrentCircleIndex;
		U32 mCurrentRectangleIndex;
		std::vector<sf::CircleShape> mCircles;
		std::vector<sf::RectangleShape> mRectangles;
};

} // namespace en

#endif // ENLIVE_DEBUG