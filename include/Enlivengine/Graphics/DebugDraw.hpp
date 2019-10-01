#pragma once

#include <Enlivengine/System/PrimitiveTypes.hpp>

#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/RenderTarget.hpp>

#include <SFML/Graphics/CircleShape.hpp>
#include <SFML/Graphics/RectangleShape.hpp>

#include <Enlivengine/Graphics/Color.hpp>
#include <Enlivengine/Math/Rect.hpp>

namespace en
{

/*

class DebugDraw //: public Singleton<DebugDraw>
{
	public:
		DebugDraw();
		
		static DebugDraw& getSingleton();
		static DebugDraw* getSingletonPtr();

		static bool instanced();

		static void clear();

		static void drawPoint(F32 x, F32 y, const Color& color = Color::Red, F32 r = 2.0f);
		static void drawPoint(const oe::Vector2& point, const Color& color = Color::Red, F32 r = 2.0f);
		static void drawRect(F32 x, F32 y, F32 w, F32 h, const Color& c1 = Color::Red, const Color& c2 = Color::Transparent);
		static void drawRect(const Rect& rect, const Color& c1 = Color::Red, const Color& c2 = Color::Transparent);
		static void drawCircle(F32 x, F32 y, const Color& color = Color::Red, F32 r = 10.f); // TODO : Improve
		static void drawLine(F32 x1, F32 y1, F32 x2, F32 y2, const Color& color = Color::Red, F32 thickness = 2.0f); // TODO : Improve

		static void render(sf::RenderTarget& target);

		static void setVisible(bool visible);
		static bool isVisible();
		
	private:
		std::vector<sf::Drawable*> mDrawables;
		bool mVisible;
};

*/

} // namespace en