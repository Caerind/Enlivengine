#include <Enlivengine/Graphics/DebugDraw.hpp>

#include <Enlivengine/Utils/Assert.hpp>

namespace en
{

void DebugDraw::DrawLine(const Vector2f& p1, const Vector2f& p2, const Color& color)
{
    AddVertex2D(p1, color);
    AddVertex2D(p2, color);
}

void DebugDraw::DrawRect(const Rectf& rect, const Color& color)
{
    const auto p0 = rect.GetCorner(0);
    const auto p1 = rect.GetCorner(1);
    const auto p2 = rect.GetCorner(2);
    const auto p3 = rect.GetCorner(3);
    
    DrawLine(p0, p1, color);
    DrawLine(p1, p2, color);
    DrawLine(p2, p3, color);
    DrawLine(p3, p0, color);
}

void DebugDraw::Render(RenderTarget& target)
{
    target.GetSFMLTarget()->draw(&mVertices2D[0], mVertices2D.size(), sf::PrimitiveType::Lines);
    mVertices2D.clear();
}

void DebugDraw::AddVertex2D(const Vector2f& pos, const Color& color)
{
    mVertices2D.push_back(sf::Vertex(sf::Vector2f(pos.x, pos.y), sf::Color(color.r, color.g, color.b, color.a)));
}

} // namespace en