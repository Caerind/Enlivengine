#pragma once

#include <Enlivengine/Math/Color.hpp>
#include <Enlivengine/Math/Vector2.hpp>
#include <Enlivengine/Math/Rect.hpp>

#include <Enlivengine/Graphics/RenderTarget.hpp>

namespace en
{

class DebugDraw
{
public:
	DebugDraw() = default;
	
	void DrawLine(const Vector2f& p1, const Vector2f& p2, const Color& color = Colors::Magenta);
	void DrawRect(const Rectf& rect, const Color& color = Colors::Magenta);

	void Render(RenderTarget& target);

private:
	void AddVertex2D(const Vector2f& pos, const Color& color);

private:
    std::vector<sf::Vertex> mVertices2D;
};

} // namespace en